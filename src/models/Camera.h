//
// Created by teteo on 03.04.2021.
//

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
    pixel* buffer;
    Point3 localization;
    Vector forward;
    Vector upward;
    Vector right;
    Line** rays;
    std::vector<std::thread> _controller;
    std::condition_variable _caller;
    std::mutex _safety;
    std::condition_variable _returner;
    std::mutex _pickup;
    std::shared_ptr<Scene> scene;
    double viewAngle;
    int _width;
    int _height;

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
    std::shared_ptr<Scene> getScene(){ return scene;}
    pixel* takePhoto();

private:
    [[noreturn]] void threadHandler(unsigned int thread_id);
    [[nodiscard]] Line generateRay(int x, int y) const;
    color_t handleRay(unsigned int x, unsigned int y);
};



#endif //RAYMARCHING_CAMERA_H
