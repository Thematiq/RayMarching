#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H
#include <cfloat>
#include <cmath>
#include "colors.h"
#include "algebra.h"

class SDFObject {
public:
    virtual ~SDFObject() = default;
    virtual double getDist(Point3 const &p) const = 0;
    virtual color_t getColor() const = 0;
};

class Shape : public SDFObject {
protected:
    Point3 _pos = Point3();
    color_t _color = BLACK;
public:
    explicit Shape(Point3 p) : _pos(p) {};
    [[nodiscard]] virtual Point3 getPos() const { return _pos; };
    [[nodiscard]] color_t getColor() const override { return _color; }
    void setColor(color_t color){ _color = color; }
};

class Sphere : public Shape {
private:
    double _radius;
public:
    explicit Sphere(Point3 p, double r = 0) : Shape(p), _radius(r) {};
    [[nodiscard]] double getDist(const Point3 &p) const override;
    [[nodiscard]] double getRadius() const { return _radius; }
};

class Cube : public Shape {
private:
    Point3 _bound;
public:
    explicit Cube(Point3 p, Point3 bound) : Shape(p), _bound(bound) {};
    [[nodiscard]] double getDist(const Point3 &p) const override;
    [[nodiscard]] Point3 getBound() const { return _bound; }
};

#endif //RAYMARCHING_MODELS3_H
