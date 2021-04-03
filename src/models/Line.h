//
// Created by teteo on 03.04.2021.
//

#ifndef RAYMARCHING_LINE_H
#define RAYMARCHING_LINE_H

#include <iostream>
#include "models3.h"

class Line {
private:
    Point3 begin;
    double xShift;
    double yShift;
    double zShift;
    double t;

public:
    Line(): begin(Point3()), t{0} {};

    Line(Point3 begin, Point3 direction): begin(begin), t{0} {
        begin.print();
        xShift = direction.getX() - begin.getX();
        yShift = direction.getY() - begin.getY();
        zShift = direction.getZ() - begin.getZ();
    }

    Line& operator=(const Line &line)= default;

    void moveBy(double distance);
    void changeDirection(){ this->t = -this->t; }
    void print();

    Point3 getPoint3();
    Point3 getPoint3(double shift);
    Line getPerpendicular(Point3 p);
};


#endif //RAYMARCHING_LINE_H
