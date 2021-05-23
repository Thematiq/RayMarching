#include "models3.h"

double Sphere::getDist(const Point3 &p) const {
    return (p - _pos).getNorm() - _radius;
}

double Cube::getDist(const Point3 &p) const {
    Point3 val = abs(p) - _bound;
    return val.getPositive().getNorm();
}

