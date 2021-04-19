#include <cmath>
#include "models3.h"

double Point3::getLargeNorm() const {
    return _x*_x + _y*_y + _z*_z;
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




void Vector::setLength(double length) {
    double vectorLength = this->getLength();
    x = length * x / vectorLength;
    y = length * y / vectorLength;
    z = length * z / vectorLength;
}

Point3 Vector::movePoint(const Point3 &p) const {
    return Point3(p.getX() + x, p.getY() + y, p.getZ() + z);
}

Vector Vector::add(const Vector &v) const {
    return Vector(x + v.getX(), y + v.getY(), z + v.getZ());
}

Vector Vector::extend(double m) const {
    return Vector(x * m, y * m, z * m);
}

Vector Vector::cross(const Vector &v) const {
    return Vector(
            y * v.getZ() - z * v.getY(),
            z * v.getX() - x * v.getZ(),
            x * v.getY() - y * v.getX());
}

double Vector::dot(const Vector &v) const {
    return x * v.getX() + y * v.getY() + z * v.getZ();
}

Vector Vector::versor() {
    Vector *v = this;
    v->setLength(1);
    return *v;
}

Vector Vector::perpendicular(Point3 &onLine, Point3 &direction) const {
    double factor = ((
                x * (direction.getX() - onLine.getX()) +
                y * (direction.getY() - onLine.getY()) +
                z * (direction.getZ() - onLine.getZ())) /
            (x * x + y * y + z * z)
            );
    return Vector(this->extend(factor).movePoint(onLine), direction);
}




void Line::moveBy(double distance) {
    this->t += distance / direction.getLength();
}

Point3 Line::getPoint3() {
    return direction.extend(t).movePoint(begin);
}

double Sphere::getDist(const Point3 &p) const {
    return (p - _pos).getNorm() - _radius;
}