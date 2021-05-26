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
    _execute = true;
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
    _controllerInit = std::vector<std::thread>(_totalThreads);
    _controllerDraw = std::vector<std::thread>(_totalThreads);
    _scene = std::make_shared<Scene>();

    _forward = (direction - localization).normalized();
    _upward = getPerpendicular(_forward, localization, up).normalized();
    _right = _forward.cross(_upward).normalized();

    _rays = new Line*[_height];
    for (unsigned int i = 0; i < _height; ++i) {
        _rays[i] = (Line*) calloc(_width, sizeof(Line));
    }
    _threadsReady = _totalThreads;
    _rowsDrawed = 0;
    _execute = false;

    for (int i = 0; i < _totalThreads; ++i) {
        _controllerInit[i] = std::thread(&Camera::initHandler, this);
        _controllerDraw[i] = std::thread(&Camera::drawingHandler, this);
    }
    for (unsigned int i = 0; i < _height; ++i) {
        std::lock_guard<std::mutex> lk(_queueMutex);
        _freeRows.push(i);
        _queueInitCond.notify_one();
    }
    std::unique_lock<std::mutex> ck(_callerMutex);
    _caller.wait(ck, [this] { return _rowsDrawed == _height; });
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
    // Assure preparedness
    _rowsDrawed = 0;
#ifdef DEBUG
    std::cout << "DRAWING READY" << std::endl;
#endif
    for (unsigned int y = 0; y < _height; ++y) {
        std::lock_guard<std::mutex> lk(_queueMutex);
        _freeRows.push(y);
        _queueDrawCond.notify_one();
    }
    std::unique_lock<std::mutex> ck(_callerMutex);
    _caller.wait(ck, [this] { return _rowsDrawed == _height; });
#ifdef DEBUG
    std::cout << std::endl << "==========" << std::endl;
#endif
    return _buffer;
}

void Camera::initHandler() {
    while (!_execute) {
        std::unique_lock<std::mutex> lk(_queueMutex);
        _queueInitCond.wait(lk, [this] { return _execute || !_freeRows.empty(); });
        if (_execute) {
            break;
        }
        unsigned int y = _freeRows.front();
        _freeRows.pop();
        lk.unlock();
        for (unsigned int x = 0; x < _width; ++x) {
            _rays[y][x] = generateRay(x, y);
        }
        _rowsDrawed++;
        _caller.notify_one();
    }
}

void Camera::drawingHandler() {
    while (!_execute) {
        std::unique_lock<std::mutex> lk(_queueMutex);
        _queueDrawCond.wait(lk, [this] { return _execute || !_freeRows.empty(); });
        if (_execute) {
            break;
        }
        unsigned int y = _freeRows.front();
        _freeRows.pop();
        lk.unlock();
        for (unsigned int x = 0; x < _width; ++x) {
            color_t color = handleRay(x, y);
            applyColor(_buffer, x, y, color);
        }
        _rowsDrawed++;
        _caller.notify_one();
    }
}