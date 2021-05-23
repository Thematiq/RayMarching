#ifndef RAYMARCHING_ALGEBRA_H
#define RAYMARCHING_ALGEBRA_H

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
    [[nodiscard]] Point3 getPositive() const;
};

Point3 abs(const Point3 &p);
Point3 operator+(const Point3 &p1, const Point3 &p2);
Point3 operator-(const Point3 &p1);
Point3 operator-(const Point3 &p1, const Point3 &p2);
bool operator==(const Point3 &lhs, const Point3 &rhs);

class Vector : public Point3 {
public:
    explicit Vector(double x = 0, double y = 0, double z = 0) : Point3(x, y, z) {};
    Vector(const Point3 begin, const Point3 end) : Point3(end - begin) {}
    [[nodiscard]] double getLength() const { return getNorm(); }
    void setLength(double length);
    Vector& operator=(const Vector &v) = default;
    [[nodiscard]] Point3 movePoint(const Point3 &p) const;
    [[nodiscard]] Vector add(const Vector &v) const;
    [[nodiscard]] Vector extend(double m) const;
    [[nodiscard]] Vector cross(const Vector &v) const;
    Vector versor();
    Vector perpendicular(Point3 &pOnLine, Point3 &direction) const;
    [[nodiscard]] double dot(const Vector &v) const;
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



#endif //RAYMARCHING_ALGEBRA_H
