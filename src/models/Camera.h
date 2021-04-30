//
// Created by teteo on 03.04.2021.
//

#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "memory"
#include "scene.h"
#include "models3.h"

#define PI 3.14159265
#define WIDTH 1280
#define HEIGHT 720
#define VIEW_ANGLE 60
#define MAX_STEPS 100
#define EPSILON 0.01

using pixel = unsigned char;

class Camera {
private:
    pixel* buffer;
    Point3 localization;
    Vector forward;
    Vector upward;
    Vector right;
    Line** rays;
    std::shared_ptr<Scene> scene;
    double viewAngle;
    int width;
    int height;

    Line generateRay(int x, int y) const;
    void handleRay(int x, int y) {
        // std::cout << x << ", " << y << std::endl;
        Line ray = rays[y][x];
        for(int step = 0; step < MAX_STEPS; step++){
            std::pair<double, Shape *> pair = scene->signedPairFunction(ray.getPoint3());
            // std::cout << pair.first << std::endl;
            if(pair.first < EPSILON){
                color_t color = pair.second->getColor();
                buffer[3 * (y * WIDTH + x)] = color.R;
                buffer[3 * (y * WIDTH + x) + 1] = color.G;
                buffer[3 * (y * WIDTH + x) + 2] = color.B;
                return;
            }
            ray.moveBy(pair.first);
        }
        buffer[3 * (y * WIDTH + x)] = WHITE.R;
        buffer[3 * (y * WIDTH + x) + 1] = WHITE.G;
        buffer[3 * (y * WIDTH + x) + 2] = WHITE.B;
    }

public:
    Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle = VIEW_ANGLE, int width = WIDTH, int height = HEIGHT);
    std::shared_ptr<Scene> getScene(){ return scene;}
    pixel* takePhoto(){
        for(int x = 0; x < WIDTH; x++){
            for(int y = 0; y < HEIGHT; y++){
                handleRay(x, y);
            }
        }
        return buffer;
    }
};



#endif //RAYMARCHING_CAMERA_H
