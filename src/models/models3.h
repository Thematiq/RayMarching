//
// Created by thmtt on 23/03/2021.
//

#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H
#include <cfloat>

/**
 * Class representing point (or vector) in 3D space
 */
class Point3 {
private:
    double x;
    double y;
    double z;
public:
    /**
     * Creates point at (0, 0, 0)
     */
    Point3() = default;
    Point3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

    /**
     * Norm without square root
     * @return norm of the vector to the second power
     */
    double getLargeNorm() const;

    /**
     * Norm of the vector associated with the point
     * @return norm value
     */
    double getNorm() const;
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
};

Point3 operator+(const Point3 &p1, const Point3 &p2);

Point3 operator-(const Point3 &p1);

Point3 operator-(const Point3 &p1, const Point3 &p2);

bool operator==(const Point3 &lhs, const Point3 &rhs);

class Shape {
protected:
    Point3 pos = Point3();
public:
    explicit Shape(Point3 p) : pos(p) {};
    virtual ~Shape() = default;
    virtual double getDist(Point3 const &p) const { return DBL_MAX; };
    virtual Point3 getPos() const { return pos; };
};

class Sphere : public Shape {
private:
    double radius;
public:
    explicit Sphere(Point3 p, double r = 0) : Shape(p), radius(r) {};
    double getDist(const Point3 &p) const override;
    double getRadius() const { return radius; }
};

#endif //RAYMARCHING_MODELS3_H
