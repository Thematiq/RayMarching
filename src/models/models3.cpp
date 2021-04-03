#include <cmath>
#include "models3.h"
#include <iostream>

double Point3::getLargeNorm() const {
    return x*x + y*y + z*z;
}

double Point3::getNorm() const {
    return sqrt(this->getLargeNorm());
}

void Point3::print() {
    std::cout << "[" << x << ", " << y << ", " << z << "]" << std::endl;
}

Point3 &Point3::operator=(const Point3 &p1) {
    x = p1.x;
    y = p1.y;
    z = p1.z;
    return *this;
}


Point3 operator-(const Point3 &p1) {
    return Point3(
            -p1.getX(),
            -p1.getY(),
            -p1.getZ());
}

Point3 operator-(const Point3 &p1, const Point3 &p2) {
    return Point3(
            p1.getX() - p2.getX(),
            p1.getY() - p2.getY(),
            p1.getZ() - p2.getZ());
}

bool operator==(const Point3 &lhs, const Point3 &rhs) {
    return  lhs.getX() == rhs.getX() &&
            lhs.getY() == rhs.getY() &&
            lhs.getZ() == rhs.getZ();
}

Point3 operator+(const Point3 &p1, const Point3 &p2) {
    return Point3(
            p1.getX() + p2.getX(),
            p1.getY() + p2.getY(),
            p1.getZ() + p2.getZ());
}
