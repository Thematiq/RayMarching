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
    std::unique_lock<std::mutex> lk(_safety);
    _terminate = true;
    lk.unlock();
    _caller.notify_all();

    for (unsigned int i = 0; i < _width; ++i) {
        free(_rays[i]);
    }
    delete[] _rays;
    delete[] _buffer;
}

Camera::Camera(Vector3d localization, const Vector3d& direction, Vector3d up, double viewAngle, int width, int height, int threads, bool use_interlacing)
        : _localization(std::move(localization)), _viewAngle(viewAngle), _width(width), _height(height), _interlacing(use_interlacing),
        _totalThreads(threads < 0 ? std::thread::hardware_concurrency() : threads) {
    _buffer = new pixel [3 * width * height];
    _controller = std::vector<std::thread>(_totalThreads);
    _scene = std::make_shared<Scene>();

    _forward = (direction - localization).normalized();
    _upward = getPerpendicular(_forward, localization, up);
    _right = _forward.cross(_upward).normalized();

    _rays = new Line*[_height];

    for (int i = 0; i < _totalThreads; ++i) {
        _controller[i] = std::thread(&Camera::threadHandler, this, i, use_interlacing);
    }

    _threadsReady = 0;
    while (true) {
        std::unique_lock<std::mutex> lk(_safety);
        _returner.wait(lk);
        if (_totalThreads == _threadsReady) {
            break;
        }
        lk.unlock();
    }
}

Line Camera::generateRay(unsigned int x, unsigned int y) const {
    double angleHorizontal = (x - (double)(_width - 1) / 2) * _viewAngle / _width;
    double angleVertical = - (y - (double)(_height - 1) / 2) * _viewAngle / _width;
    Vector3d pixelPoint = _localization;
    pixelPoint = (_forward * (cos(angleHorizontal * HALF_CIRCLE) * cos(angleVertical * HALF_CIRCLE))) + pixelPoint;
    pixelPoint = (_right * (sin(angleHorizontal * HALF_CIRCLE) * cos(angleVertical * HALF_CIRCLE))) + pixelPoint;
    pixelPoint = (_upward * sin(angleVertical * HALF_CIRCLE)) + pixelPoint;
    Vector3d vec = pixelPoint - _localization;
    return Line(vec, _localization);
}

color_t Camera::handleRay(unsigned int x, unsigned int y) const {
    Line ray = _rays[y][x];
    for(int step = 0; step < MAX_STEPS; step++){
        shapeDist pair = _scene->signedPairFunction(ray.getVec());
        if(pair.first < EPSILON){
            return pair.second->getColor();
        }
        ray.moveBy(pair.first);
    }
    return BACKGROUND;
}

pixel* Camera::takePhoto() {
    _threadsReady = 0;
    _caller.notify_all();
    while (true) {
        std::unique_lock<std::mutex> lk(_pickup);
        _returner.wait(lk);
        if (_threadsReady == _totalThreads) {
            break;
        }
        lk.unlock();
    }

    return _buffer;
}

void Camera::threadHandler(unsigned int thread_id, bool use_interlacing) {
    bool interlace_odd = false;
    const unsigned int increment_val = use_interlacing ? 2 : 1;
    const unsigned int top_row = ceil(thread_id * (double)(_height) / _totalThreads);
    const unsigned int bottom_row = ceil((thread_id + 1) * (double)(_height) / _totalThreads);
    // Init system
    for (unsigned int y = top_row; y < bottom_row; ++y) {\
        // We do not want to initialize Lines
        _rays[y] = (Line*) calloc(_width, sizeof(Line));
        for (unsigned int x = 0; x < _width; ++x) {
            _rays[y][x] = generateRay(x, y);
        }
    }
    std::unique_lock<std::mutex> lk(_safety);
    _threadsReady++;
    lk.unlock();
    _returner.notify_one();

    // Await further instructions
    while (true) {
        std::unique_lock<std::mutex> lk(_safety);
        _caller.wait(lk);
        bool status = _terminate;
        lk.unlock();
        if (status) {
            std::terminate();
        }
        for (unsigned int y = top_row + (int)(interlace_odd); y < bottom_row; y += increment_val) {
            for (unsigned int x = 0; x < _width; ++x) {
                color_t col = handleRay(x, y);
                applyColor(_buffer, x, y, col);
            }
            if (use_interlacing) {
                interlace_odd = !interlace_odd;
            }
        }
        lk.lock();
        _threadsReady++;
        lk.unlock();
        _returner.notify_one();
    }
}
