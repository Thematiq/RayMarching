#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H
#include <cfloat>
#include <cmath>
#include "const.h"
#include "colors.h"
#include "algebra.h"
#include <Eigen/Dense>

class SDFObject {
public:
    virtual ~SDFObject() = default;
    virtual double getDist(Eigen::Vector3d const &p) const = 0;
    virtual color_t getColor() const = 0;
    virtual Line getReflection(Line& ray) const = 0;
};

class Shape : public SDFObject {
protected:
    Eigen::Vector3d _pos;
    color_t _color = BLACK;
public:
    explicit Shape(Eigen::Vector3d p) : _pos(std::move(p)) {};
    [[nodiscard]] virtual Eigen::Vector3d getPos() const { return _pos; };
    [[nodiscard]] color_t getColor() const override { return _color; }
    void setColor(color_t color){ _color = color; }
    [[nodiscard]] Line getReflection(Line& ray) const override;;
};

class Sphere : public Shape {
private:
    double _radius;
public:
    explicit Sphere(Eigen::Vector3d p, double r = 0) : Shape(std::move(p)), _radius(r) {};
    [[nodiscard]] double getDist(const Eigen::Vector3d &p) const override;
    [[nodiscard]] double getRadius() const { return _radius; }
};

class Cube : public Shape {
private:
    Eigen::Vector3d _bound;
public:
    explicit Cube(Eigen::Vector3d p, double bound) : Shape(std::move(p)), _bound(bound, bound, bound) {};
    [[nodiscard]] double getDist(const Eigen::Vector3d &p) const override;
    [[nodiscard]] double getBound() const { return _bound.x(); }
};

#endif //RAYMARCHING_MODELS3_H
