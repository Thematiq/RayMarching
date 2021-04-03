

#include <cmath>
#include "Line.h"


void Line::moveBy(double distance) {
    this->t += distance / sqrt(pow(xShift, 2) + pow(yShift, 2) + pow(zShift, 2));
}


Point3 Line::getPoint3() {
    return Point3(begin.getX() + t * xShift, begin.getY() + t * yShift, begin.getZ() + t * zShift);
}

Point3 Line::getPoint3(double shift) {
    return Point3(begin.getX() + shift * xShift, begin.getY() + shift * yShift, begin.getZ() + shift * zShift);
}

Line Line::getPerpendicular(Point3 p) {
    double shift = -(begin.getX() * p.getX() + begin.getY() * p.getY() + begin.getZ() * p.getZ()) /
            (xShift * p.getX() + yShift * p.getY() + zShift * p.getZ());
    return Line(this->getPoint3(shift), p);
}

void Line::print() {
    std::cout << "Line: " << xShift << ", " << yShift << ", " << zShift << std::endl;
}




