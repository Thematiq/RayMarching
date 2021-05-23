#include "models3.h"

double Sphere::getDist(const Point3 &p) const {
    return (p - _pos).getNorm() - _radius;
}