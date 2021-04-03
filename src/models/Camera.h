//
// Created by teteo on 03.04.2021.
//

#ifndef RAYMARCHING_CAMERA_H
#define RAYMARCHING_CAMERA_H

#include <cmath>
#include "models3.h"
#include "Line.h"

#define PI 3.14159265
#define PIXEL_DIMENSION 0.000001

using pixel = unsigned char;

class Camera {
private:
    pixel* buffer;
    Point3 localization;
    Point3 backLocalization;
    Point3 topLocalization;
    int width;
    int height;

public:
    Camera(Point3 localization, Point3 direction, double viewAngle, int width, int height) : localization(localization), width(width), height(height){
        Line line = Line(localization, direction);
        line.moveBy(- (PIXEL_DIMENSION * width / 2) / tan(viewAngle * PI / 180));
        backLocalization = line.getPoint3();
        topLocalization = Point3(0, 0 ,1) + localization;
        buffer = new pixel [width * height * 3];
    }

    void setTopLocalization(Point3 topLocalization) {
        this->topLocalization = topLocalization;
    }

    Line getLine(int x, int y){

        Point3 direction = Point3();
        Line line = Line(direction, backLocalization);
        line.changeDirection();
        return line;
    }
};



#endif //RAYMARCHING_CAMERA_H
