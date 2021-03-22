#include <cmath>
#include "models2.h"

double Point2::getLargeNorm() const {
    return x*x + y*y;
}

double Point2::getNorm() const {
    return sqrt(this->getLargeNorm());
}

Point2 operator+(const Point2 &p1, const Point2 &p2) {
    return Point2(
            p1.getX() + p2.getX(),
            p1.getY() + p2.getY());
}

Point2 operator-(const Point2 &p1) {
    return Point2(
            -p1.getX(),
            -p1.getY());
}

Point2 operator-(const Point2 &p1, const Point2 &p2) {
    return Point2(
            p1.getX() - p2.getX(),
            p1.getY() - p2.getY());
}

bool operator==(const Point2 &lhs, const Point2 &rhs) {
    return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}

double Circle::getDist(Point2 viewer) {
    return (center - viewer).getNorm() - radius;
}