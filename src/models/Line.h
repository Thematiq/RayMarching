//
// Created by teteo on 03.04.2021.
//

#ifndef RAYMARCHING_LINE_H
#define RAYMARCHING_LINE_H

#include "models3.h"

class Line {
private:
    Point3 begin;
    Vector direction;
    double t;
    int phrase;

public:
    Line(Point3 begin, Vector direction): begin(begin), direction(direction), t(0), phrase(1) {}

    Line& operator=(const Line &line){
        begin = line.begin;
        direction = line.direction;
        t = 0;
        return *this;
    }

    void moveBy(double distance);
    void changeDirection(){ phrase *= -1; }

    Point3 getPoint3();
    Line refractionBy(double horizontal, double vertical, double rotation);
};


#endif //RAYMARCHING_LINE_H
