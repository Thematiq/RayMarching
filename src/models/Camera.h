//
// Created by teteo on 03.04.2021.
//

#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "models3.h"
#include "Line.h"

#define PI 3.14159265
#define CAMERA_SIZE 0.000001

using pixel = unsigned char;

class Camera {
private:
    pixel* buffer;
    Point3 localization;
    Vector forward;
    Vector upward;
    double viewAngle;
    int width;
    int height;

public:
    Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle, int width, int height)
            : localization(localization), viewAngle(viewAngle), width(width), height(height), forward(localization, direction){

        Point3 p0 = Point3(7, 3, -4);
        Point3 p  = Point3(-2,-1, 6);

        Vector u = Vector(-3, 2, 5);

        double a = (u.getX() * (p.getX() - p0.getX()) + u.getY() * (p.getY() - p0.getY()) + u.getZ() * (p.getZ() - p0.getZ())) /
                (u.getX() * u.getX() + u.getY() * u.getY() + u.getZ() * u.getZ());

        Vector shift = u.extend(a);
        Point3 A = shift.movePoint(p0);
        std::cout << "a = " << a << std::endl << "Point A: " << A.getX() << ", " << A.getY() << ", " << A.getZ() << std::endl;

        Vector AP = Vector(A, p);
        std::cout << "Vector AP: " << AP.getX() << ", " << AP.getY() << ", " << AP.getZ() << std::endl;

        double dot_p = AP.dot(u);

        std::cout << "dot: " << dot_p << std::endl;

        buffer = new pixel [width * height * 3];
    }

};



#endif //RAYMARCHING_CAMERA_H
