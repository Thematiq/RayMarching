#include "Camera.h"

constexpr double HALF_CIRCLE = M_PI / 180;
const color_t Camera::BACKGROUND = WHITE;
using namespace Eigen;

// Assuming consts
inline
unsigned int posToBuf(const unsigned int &x, const unsigned int &y) {
    return 3 * (x + WIDTH * y);
}

inline
void applyColor(pixel* buffer, const unsigned int &x, const unsigned int &y, const color_t &c) {
    buffer[posToBuf(x, y)] = c.R;
    buffer[posToBuf(x, y) + 1] = c.G;
    buffer[posToBuf(x, y) + 2] = c.B;
}

inline
Vector3d getPerpendicular(Vector3d vec, const Vector3d& onLine, const Vector3d& direction) {
    double factor = ((direction - onLine).array() * vec.array()).sum() / vec.squaredNorm();
    return direction -  ((vec * factor) + onLine);
}

Camera::~Camera() {
    for (unsigned int i = 0; i < _width; ++i) {
        free(_rays[i]);
    }
    delete[] _rays;
    delete[] _buffer;
}

Camera::Camera(Vector3d localization, const Vector3d& direction, const Vector3d& up, double viewAngle, int width, int height, int threads, bool use_interlacing)
        : _localization(std::move(localization)), _viewAngle(viewAngle), _width(width), _height(height), _interlace(use_interlacing),
          _jumpDist(use_interlacing ? 2 : 1), _evenFrame(false), _totalThreads(threads < 0 ? std::thread::hardware_concurrency() : threads),
          _awaitFor(use_interlacing ?  height / 2 : height) {
    if (use_interlacing && (height % 2 != 0)) {
        throw std::invalid_argument("Height must be multiple of two for interlacing!");
    }
    _buffer = new pixel [3 * width * height];
    _drones = std::vector<std::thread>(_totalThreads);
    _scene = std::make_shared<Scene>();

    _forward = (direction - localization).normalized();
    _upward = getPerpendicular(_forward, localization, up).normalized();
    _right = _forward.cross(_upward).normalized();

    _rays = new Line*[_height];
    for (unsigned int i = 0; i < _height; ++i) {
        _rays[i] = (Line*) calloc(_width, sizeof(Line));
    }
    for (unsigned int i = 0; i < _totalThreads; ++i) {
        _drones[i] = std::thread(&Camera::threadHandler, this);
    }
    applyCommand(CameraCommands::GENERATE);
    if (_interlace) {
        applyCommand(CameraCommands::GENERATE);
    }
}

Line Camera::generateRay(unsigned int x, unsigned int y) const {
    double angleHorizontal = (x - (double)(_width - 1) / 2) * _viewAngle / _width;
    double angleVertical = (y - (double)(_height - 1) / 2) * _viewAngle / _width;
    Vector3d pixelPoint = _localization;
    pixelPoint = (_forward * (cos(angleHorizontal * HALF_CIRCLE) * cos(angleVertical * HALF_CIRCLE))) + pixelPoint;
    pixelPoint = (_right * (sin(angleHorizontal * HALF_CIRCLE) * cos(angleVertical * HALF_CIRCLE))) + pixelPoint;
    pixelPoint = (_upward * sin(angleVertical * HALF_CIRCLE)) + pixelPoint;
    Vector3d vec = pixelPoint - _localization;
    return Line(vec, _localization);
}

color_t Camera::handleRay(Line& ray, color_t color, unsigned int reflection) const{
    for(int step = 0; step < MAX_STEPS; step++){
        shapeDist pair = _scene->signedPairFunction(ray.getVec());

        if(pair.first < EPSILON){
            Line reflected_ray = pair.second->getReflection(ray);
            double factor = (1.5 - 0.5 *ray.getDirection().dot(reflected_ray.getDirection()) /
                                 (ray.getDirection().norm() * reflected_ray.getDirection().norm())) / 2;
            color.R = (pixel)((1 - factor) * color.R + factor * pair.second->getColor().R);
            color.G = (pixel)((1 - factor) * color.G + factor * pair.second->getColor().G);
            color.B = (pixel)((1 - factor) * color.B + factor * pair.second->getColor().B);

            if(reflection == MAX_REFLECTIONS){
                return color;
            }
            else{
                reflected_ray.moveBy(EPSILON);
                return handleRay(reflected_ray, color, reflection + 1);
            }
        }
        ray.moveBy(pair.first);
        if((_localization - ray.getVec()).norm() > MAX_DISTANCE){ break; }
    }
    if(reflection > 0){ return  color; }
    return BACKGROUND;
}

pixel* Camera::takePhoto() {
    applyCommand(CameraCommands::DRAW);
    return _buffer;
}

void Camera::setCamera(const Vector3d &localization, const Vector3d &direction, const Vector3d &up) {
    _localization = localization;
    _forward = (direction - _localization).normalized();
    _upward = getPerpendicular(_forward, _localization, up).normalized();
    _right = _forward.cross(_upward).normalized();
    applyCommand(CameraCommands::GENERATE);
    if (_interlace) {
        applyCommand(CameraCommands::GENERATE);
    }
}

void Camera::applyCommand(Camera::CameraCommands cmd) {
    _cn.set(0);
    for (auto i = (unsigned int)(_evenFrame); i < _height; i += _jumpDist) {
        _queue.enqueue(command_pair(cmd, i));
    }
    _cn.await_for(_awaitFor);
    _evenFrame = _interlace && (!_evenFrame);
}

void Camera::threadHandler() {
    while (true) {
        command_pair cp = _queue.deque();
        unsigned int y = cp.second;
        for (unsigned int x = 0; x < _width; ++x) {
            switch (cp.first) {
                case CameraCommands::GENERATE:
                    _rays[y][x] = generateRay(x, y);
                    break;
                case CameraCommands::DRAW:
                    applyColor(_buffer, x, y, handleRay(_rays[y][x]));
                    break;
            }
        }
        ++_cn;
    }
}


