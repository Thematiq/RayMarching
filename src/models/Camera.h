//
// Created by teteo on 03.04.2021.
//

#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "models3.h"

#define PI 3.14159265

using pixel = unsigned char;

class Camera {
private:
    pixel* buffer;
    Point3 localization;
    Vector forward;
    Vector upward;
    Vector right;
    Line** rays;
    double viewAngle;
    int width;
    int height;

    Line generateRay(int x, int y) const;

public:
    Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle, int width, int height);
    Line getRay(int x, int y){ return rays[x][y]; }
};



#endif //RAYMARCHING_CAMERA_H
