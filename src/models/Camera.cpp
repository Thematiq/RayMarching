#include "Camera.h"

const color_t Camera::BACKGROUND = WHITE;

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

Camera::Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle, int width, int height, int threads)
        : localization(localization), viewAngle(viewAngle), _width(width), _height(height),
        _controller(threads < 0 ? std::thread::hardware_concurrency() : threads) {
    buffer = new pixel [3 * width * height];
    scene = std::make_shared<Scene>();

    forward = Vector(localization, direction).versor();
    upward = forward.perpendicular(localization, up).versor();
    right = forward.cross(upward).versor();

    rays = (Line **) malloc(sizeof(Line *) * height);
    for(int y = 0; y < height; y++){
        rays[y] = (Line *) malloc(sizeof(Line) * width);
        for(int x = 0; x < width; x++){
            rays[y][x] = generateRay(x, y);
        }
    }
    for (int i = 0; i < _controller.size(); ++i) {
        _controller[i] = std::thread(&Camera::threadHandler, this, i);
    }
}

Line Camera::generateRay(int x, int y) const {
    double angleHorizontal = (x - (double)(_width - 1) / 2) * viewAngle / _width;
    double angleVertical = - (y - (double)(_height - 1) / 2) * viewAngle / _width;
    Point3 pixelPoint = localization;
    pixelPoint = forward.extend(cos(angleHorizontal * M_PI / 180) * cos(angleVertical * M_PI / 180)).movePoint(pixelPoint);
    pixelPoint = right.extend(sin(angleHorizontal * M_PI / 180) * cos(angleVertical * M_PI / 180)).movePoint(pixelPoint);
    pixelPoint = upward.extend(sin(angleVertical * M_PI / 180)).movePoint(pixelPoint);
    Vector vec = Vector(localization, pixelPoint);
    return Line(vec, localization);
}

color_t Camera::handleRay(unsigned int x, unsigned int y) {
    Line ray = rays[y][x];
    for(int step = 0; step < MAX_STEPS; step++){
        std::pair<double, Shape *> pair = scene->signedPairFunction(ray.getPoint3());
        if(pair.first < EPSILON){
            return pair.second->getColor();
        }
        ray.moveBy(pair.first);
    }
    return BACKGROUND;
}

pixel* Camera::takePhoto() {
    _caller.notify_all();
    for (int i = 0; i < _controller.size(); ++i) {
        std::unique_lock<std::mutex> lk(_pickup);
        _returner.wait(lk);
        lk.unlock();
    }

    return buffer;
}

void Camera::threadHandler(unsigned int thread_id) {
    const unsigned int top_row = ceil(thread_id * (double)(_height) / _controller.size());
    const unsigned int bottom_row = ceil((thread_id + 1) * (double)(_height) / _controller.size());
    while (true) {
        std::unique_lock<std::mutex> lk(_safety);
        _caller.wait(lk);

        lk.unlock();
        for (unsigned int y = top_row; y < bottom_row; ++y) {
            for (unsigned int x = 0; x < _width; ++x) {
                color_t col = handleRay(x, y);
                applyColor(buffer, x, y, col);
            }
        }
        _returner.notify_one();
    }
}
