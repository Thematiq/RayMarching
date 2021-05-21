#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "memory"
#include "scene.h"
#include "models3.h"
#include "const.h"
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
    bool _terminate = false;
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
    Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle = VIEW_ANGLE, int width = WIDTH, int height = HEIGHT, int thread = -1);
    ~Camera();
    std::shared_ptr<Scene> getScene(){ return _scene;}
    pixel* takePhoto();

private:
    [[noreturn]] void threadHandler(unsigned int thread_id);
    [[nodiscard]] Line generateRay(unsigned int x, unsigned int y) const;
    [[nodiscard]] color_t handleRay(unsigned int x, unsigned int y) const;
};



#endif //RAYMARCHING_CAMERA_H
