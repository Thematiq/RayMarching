#include "algebra.h"

double Point3::getLargeNorm() const {
    return _x*_x + _y*_y + _z*_z;
}

double Point3::getNorm() const {
    return sqrt(this->getLargeNorm());
}

void Point3::print() const {
    std::cout << "[" << _x << ", " << _y << ", " << _z << "]" << std::endl;
}

Point3 Point3::getPositive() const {
    return Point3(
            _x > 0 ? _x : 0,
            _y > 0 ? _y : 0,
            _z > 0 ? _z : 0
    );
}

Point3 &Point3::operator=(const Point3 &p1) {
    _x = p1._x;
    _y = p1._y;
    _z = p1._z;
    return *this;
}

Point3 abs(const Point3 &p) {
    return Point3(
            std::abs(p.getX()),
            std::abs(p.getY()),
            std::abs(p.getZ())
            );
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
    _x = length * _x / vectorLength;
    _y = length * _y / vectorLength;
    _z = length * _z / vectorLength;
}

Point3 Vector::movePoint(const Point3 &p) const {
    return Point3(p.getX() + _x, p.getY() + _y, p.getZ() + _z);
}

Vector Vector::add(const Vector &v) const {
    return Vector(_x + v.getX(), _y + v.getY(), _z + v.getZ());
}

Vector Vector::extend(double m) const {
    return Vector(_x * m, _y * m, _z * m);
}

Vector Vector::cross(const Vector &v) const {
    return Vector(
            _y * v.getZ() - _z * v.getY(),
            _z * v.getX() - _x * v.getZ(),
            _x * v.getY() - _y * v.getX());
}

double Vector::dot(const Vector &v) const {
    return _x * v.getX() + _y * v.getY() + _z * v.getZ();
}

Vector Vector::versor() {
    Vector *v = this;
    v->setLength(1);
    return *v;
}

Vector Vector::perpendicular(Point3 &onLine, Point3 &direction) const {
    double factor = ((
                             _x * (direction.getX() - onLine.getX()) +
                             _y * (direction.getY() - onLine.getY()) +
                             _z * (direction.getZ() - onLine.getZ())) /
                     getLargeNorm()
    );
    return Vector(this->extend(factor).movePoint(onLine), direction);
}




void Line::moveBy(double distance) {
    this->t += distance / direction.getLength();
}

Point3 Line::getPoint3() {
    return direction.extend(t).movePoint(begin);
}