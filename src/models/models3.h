//
// Created by thmtt on 23/03/2021.
//

#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H
#include <cfloat>
#include <cmath>
#include <iostream>

/**
 * Class representing point (or vector) in 3D space
 */
class Point3 {
protected:
    double _x;
    double _y;
    double _z;
public:
    /**
     * Creates point at (0, 0, 0)
     */
    Point3() = default;
    Point3(double x, double y, double z) : _x(x), _y(y), _z(z) {};
    void print() const;
    [[nodiscard]] double getX() const { return _x; }
    [[nodiscard]] double getY() const { return _y; }
    [[nodiscard]] double getZ() const { return _z; }
    Point3& operator=(const Point3 &p1);
    /**
     * Norm without square root
     * @return norm of the vector to the second power
     */
    [[nodiscard]] double getLargeNorm() const;
    /**
     * Norm of the vector associated with the point
     * @return norm value
     */
    [[nodiscard]] double getNorm() const;
};

Point3 operator+(const Point3 &p1, const Point3 &p2);

Point3 operator-(const Point3 &p1);

Point3 operator-(const Point3 &p1, const Point3 &p2);

bool operator==(const Point3 &lhs, const Point3 &rhs);



class Vector : public Point3 {
public:
    explicit Vector(double x = 0, double y = 0, double z = 0) : Point3(x, y, z) {};
    Vector(const Point3 begin, const Point3 end) {
        _x = end.getX() - begin.getX();
        _y = end.getY() - begin.getY();
        _z = end.getZ() - begin.getZ();
    }
    double getLength() const { return sqrt(_x*_x + _y*_y + _z*_z); }
    void setLength(double length);
    Vector& operator=(const Vector &v) = default;
    Point3 movePoint(const Point3 &p) const;
    Vector add(const Vector &v) const;
    Vector extend(double m) const;
    Vector cross(const Vector &v) const;
    Vector versor();
    Vector perpendicular(Point3 &pOnLine, Point3 &direction) const;
    double dot(const Vector &v) const;
};



class Line {
private:
    Point3 begin;
    Vector direction;
    double t;

public:
    Line(Vector direction, Point3 begin): begin(begin), direction(direction), t(0) {}
    void moveBy(double distance);
    Point3 getPoint3();
};

class Shape {
protected:
    Point3 _pos = Point3();
public:
    explicit Shape(Point3 p) : _pos(p) {};
    virtual ~Shape() = default;
    [[nodiscard]] virtual double getDist([[maybe_unused]] Point3 const &p) const { return DBL_MAX; };
    [[nodiscard]] virtual Point3 getPos() const { return _pos; };
};

class Sphere : public Shape {
private:
    double _radius;
public:
    explicit Sphere(Point3 p, double r = 0) : Shape(p), _radius(r) {};
    [[nodiscard]] double getDist(const Point3 &p) const override;
    [[nodiscard]] double getRadius() const { return _radius; }
};

#endif //RAYMARCHING_MODELS3_H
