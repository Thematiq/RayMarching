//
// Created by thmtt on 23/03/2021.
//

#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H

#include <iostream>

class Point3 {
private:
    double x;
    double y;
    double z;
public:
    Point3() = default;
    Point3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
    double getLargeNorm() const;
    double getNorm() const;
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    void print();

    Point3& operator=(const Point3 &p1);
};

Point3 operator+(const Point3 &p1, const Point3 &p2);

Point3 operator-(const Point3 &p1);

Point3 operator-(const Point3 &p1, const Point3 &p2);

bool operator==(const Point3 &lhs, const Point3 &rhs);



class Vector {
    double x;
    double y;
    double z;

public:
    Vector(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z){};
    Vector(const Point3 begin, const Point3 end){
        x = end.getX() - begin.getX();
        y = end.getY() - begin.getY();
        z = end.getZ() - begin.getZ();
    }

    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    double getLength() const { return sqrt(x*x + y*y + z*z); }
    void setLength(double length);

    Vector& operator=(const Vector &v) = default;

    Point3 movePoint(const Point3 &p) const;
    Vector add(const Vector &v) const;
    Vector extend(double m) const;
    Vector cross(const Vector &v) const;
    Vector versor() const;
    Vector perpendicular(Point3 &pOnLine, Point3 &direction) const;
    double dot(const Vector &v) const;
};



class Line {
private:
    Vector direction;
    Point3 begin;
    double t;

public:
    Line(): t(0){
        direction = Vector();
        begin = Point3();
    }
    Line(Vector direction, Point3 begin): begin(begin), direction(direction), t(0) {}

    Line& operator=(const Line &line);

    void moveBy(double distance);
    Point3 getPoint3();
};

#endif //RAYMARCHING_MODELS3_H
