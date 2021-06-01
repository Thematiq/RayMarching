#include "Camera.h"

namespace RayMarching {

    constexpr double HALF_CIRCLE = M_PI / 180;
    const color_t Camera::BACKGROUND = WHITE;
    using namespace Eigen;

// Assuming consts
    inline
    unsigned int posToBuf(const unsigned int &x, const unsigned int &y, Settings_t set) {
        return 3 * (x + set.width * y);
    }

    inline
    void Camera::applyColor(const unsigned int &x, const unsigned int &y, const color_t &c) {
        _buffer[posToBuf(x, y, _settings)] = c.R;
        _buffer[posToBuf(x, y, _settings) + 1] = c.G;
        _buffer[posToBuf(x, y, _settings) + 2] = c.B;
    }

    inline
    Vector3d getPerpendicular(Vector3d vec, const Vector3d &onLine, const Vector3d &direction) {
        double factor = ((direction - onLine).array() * vec.array()).sum() / vec.squaredNorm();
        return direction - ((vec * factor) + onLine);
    }

    Camera::~Camera() {
        for (unsigned int i = 0; i < _settings.width; ++i) {
            free(_rays[i]);
        }
        delete[] _rays;
        delete[] _buffer;
    }

    Camera::Camera(const Vector3d &localization, const Vector3d &direction, const Vector3d &up, Settings_t set)
            : _settings(set), _localization(localization), _jumpDist(set.interlace ? 2 : 1),
              _evenFrame(false), _awaitFor(set.interlace ? set.height / 2 : set.height) {
        if (_settings.interlace && (_settings.height % 2 != 0)) {
            throw std::invalid_argument("Height must be multiple of two for interlacing!");
        }
        _buffer = new pixel[3 * _settings.width * _settings.height];
        _drones = std::vector<std::thread>(_settings.threads);
        _scene = std::make_shared<Scene>();

        _forward = (direction - localization).normalized();
        _upward = getPerpendicular(_forward, localization, up).normalized();
        _right = _forward.cross(_upward).normalized();

        _rays = new Line *[_settings.height];
        for (unsigned int i = 0; i < _settings.height; ++i) {
            _rays[i] = (Line *) calloc(_settings.width, sizeof(Line));
        }
        for (auto &i : _drones) {
            i = std::thread(&Camera::threadHandler, this);
        }
        applyCommand(CameraCommands::GENERATE);
        if (_settings.interlace) {
            applyCommand(CameraCommands::GENERATE);
        }
    }

    Line Camera::generateRay(unsigned int x, unsigned int y) const {
        double angleHorizontal = (x - (double)(_settings.height - 1) / 2) * _settings.viewAngle / _settings.width;
        double angleVertical = (y - (double)(_settings.height - 1) / 2) * _settings.viewAngle / _settings.width;
        Vector3d pixelPoint = _localization;
        pixelPoint = (_forward * (cos(angleHorizontal * HALF_CIRCLE) * cos(angleVertical * HALF_CIRCLE))) + pixelPoint;
        pixelPoint = (_right * (sin(angleHorizontal * HALF_CIRCLE) * cos(angleVertical * HALF_CIRCLE))) + pixelPoint;
        pixelPoint = (_upward * sin(angleVertical * HALF_CIRCLE)) + pixelPoint;
        Vector3d vec = pixelPoint - _localization;
        return Line(vec, _localization);
    }

    color_t Camera::handleRay(Line& ray, color_t color, unsigned int reflection) const{
        if (reflection == 0) {
            ray.reset();
        }
        for(int step = 0; step < _settings.maxSteps; step++){
            shapeDist pair = _scene->signedPairFunction(ray.getVec());

            if(pair.first < _settings.epsilon){
                Line reflected_ray = pair.second->getReflection(ray);
                color_t current_col = pair.second->getColor(ray.getVec());
                double factor = (1.5 - 0.5 *ray.getDirection().dot(reflected_ray.getDirection()) /
                                     (ray.getDirection().norm() * reflected_ray.getDirection().norm())) / 2;
                color.R = (pixel)((1 - factor) * color.R + factor * current_col.R);
                color.G = (pixel)((1 - factor) * color.G + factor * current_col.G);
                color.B = (pixel)((1 - factor) * color.B + factor * current_col.B);

                if(reflection == _settings.maxReflections){
                    return color;
                }
                else{
                    reflected_ray.moveBy(_settings.epsilon);
                    return handleRay(reflected_ray, color, reflection + 1);
                }
            }
            ray.moveBy(pair.first);
            if((_localization - ray.getVec()).norm() > _settings.maxDistance){ break; }
        }
        if (reflection > 0){ return  color; }
        return BACKGROUND;
    }

    pixel *Camera::takePhoto() {
        applyCommand(CameraCommands::DRAW);
        return _buffer;
    }

    void Camera::setCamera(const Vector3d &localization, const Vector3d &direction, const Vector3d &up) {
        _localization = localization;
        _forward = (direction - _localization).normalized();
        _upward = getPerpendicular(_forward, _localization, up).normalized();
        _right = _forward.cross(_upward).normalized();
        applyCommand(CameraCommands::GENERATE);
        if (_settings.interlace) {
            applyCommand(CameraCommands::GENERATE);
        }
    }

    void Camera::applyCommand(Camera::CameraCommands cmd) {
        _cn.set(0);
        for (auto i = (unsigned int) (_evenFrame); i < _settings.height; i += _jumpDist) {
            _queue.enqueue(command_pair(cmd, i));
        }
        _cn.await_for(_awaitFor);
        _evenFrame = _settings.interlace && (!_evenFrame);
    }

    void Camera::threadHandler() {
        while (true) {
            command_pair cp = _queue.deque();
            unsigned int y = cp.second;
            for (unsigned int x = 0; x < _settings.width; ++x) {
                switch (cp.first) {
                    case CameraCommands::GENERATE:
                        _rays[y][x] = generateRay(x, y);
                        break;
                    case CameraCommands::DRAW:
                        applyColor(x, y, handleRay(_rays[y][x]));
                        break;
                }
            }
            ++_cn;
        }
    }
}