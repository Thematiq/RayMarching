#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H
#include <cfloat>
#include <cmath>
#include "colors.h"
#include "algebra.h"

class Shape {
protected:
    Point3 _pos = Point3();
    color_t _color = BLACK;
public:
    explicit Shape(Point3 p) : _pos(p) {};
    virtual ~Shape() = default;
    [[nodiscard]] virtual double getDist([[maybe_unused]] Point3 const &p) const { return DBL_MAX; };
    [[nodiscard]] virtual Point3 getPos() const { return _pos; };
    [[nodiscard]] color_t getColor() const { return _color; }
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

#endif //RAYMARCHING_MODELS3_H
