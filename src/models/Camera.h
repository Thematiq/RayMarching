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
    double viewAngle;
    int width;
    int height;

public:
    Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle, int width, int height)
            : localization(localization), viewAngle(viewAngle), width(width), height(height){

        forward = Vector(localization, direction).versor();
        upward = forward.perpendicular(localization, up).versor();
        right = forward.cross(upward).versor();

        std::cout << "Vector forward: " << forward.getX() << ", " << forward.getY() << ", " << forward.getZ() << std::endl;
        std::cout << "Vector upward: " << upward.getX() << ", " << upward.getY() << ", " << upward.getZ() << std::endl;
        std::cout << "Vector right: " << right.getX() << ", " << right.getY() << ", " << right.getZ() << std::endl;
        std::cout << width << ", " << height << " | " << viewAngle << std::endl;

        buffer = new pixel [width * height * 3];

        Line line;
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                line = getRay(i, j);
            }
        }

    }

    Line getRay(int x, int y) const{
        double angleHorizontal = (x - width / 2) * viewAngle / width;
        double angleVertical = - (y - height / 2) * viewAngle / width;
        std::cout << angleHorizontal << ", " << angleVertical << std::endl;
        Point3 pixelPoint = localization;
        pixelPoint = forward.extend(cos(angleHorizontal * PI / 180) * cos(angleVertical * PI / 180)).movePoint(pixelPoint);
        pixelPoint = right.extend(sin(angleHorizontal * PI / 180) * cos(angleVertical * PI / 180)).movePoint(pixelPoint);
        pixelPoint = upward.extend(sin(angleVertical * PI / 180)).movePoint(pixelPoint);

        std::cout << "Pixel point: " << pixelPoint.getX() << ", " << pixelPoint.getY() << ", " << pixelPoint.getZ() << std::endl;
        Vector vec = Vector(localization, pixelPoint);
        return Line(vec, localization);
    }

};



#endif //RAYMARCHING_CAMERA_H
