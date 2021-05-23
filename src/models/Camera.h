#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "memory"
#include "scene.h"
#include "models3.h"
#include "const.h"
#include "algebra.h"
#include <thread>
#include <vector>
#include <condition_variable>

using pixel = unsigned char;

class Camera {
private:
    const static color_t BACKGROUND;
    const double _viewAngle;
    const int _width;
    const int _height;
    const bool _interlacing;
    bool _terminate = false;
    size_t _frameID = 0;
    pixel* _buffer;
    Point3 _localization;
    Vector _forward;
    Vector _upward;
    Vector _right;
    Line** _rays;
    std::vector<std::thread> _controller;
    std::condition_variable _caller;
    std::mutex _safety;
    std::condition_variable _returner;
    std::mutex _pickup;
    std::shared_ptr<Scene> _scene;

public:
    /**
     * Spawns new camera
     * @param localization
     * @param direction
     * @param up
     * @param viewAngle
     * @param width
     * @param height
     * @param thread - number of threads, if < 0 then uses all available hardware threads
     */
    Camera(Point3 localization, Point3 direction, Point3 up, bool use_interlacing = false)
    : Camera(localization, direction, up, -1, use_interlacing) {}
    Camera(Point3 localization, Point3 direction, Point3 up, int threads, bool use_interlacing = false)
    : Camera(localization, direction, up, VIEW_ANGLE, WIDTH, HEIGHT, threads, use_interlacing) {}
    Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle, int width, int height, int thread, bool use_interlacing);
    ~Camera();
    std::shared_ptr<Scene> getScene(){ return _scene;}
    pixel* takePhoto();

private:
    [[noreturn]] void threadHandler(unsigned int thread_id, bool use_interlacing = false);
    [[nodiscard]] Line generateRay(unsigned int x, unsigned int y) const;
    [[nodiscard]] color_t handleRay(unsigned int x, unsigned int y) const;
};



#endif //RAYMARCHING_CAMERA_H
