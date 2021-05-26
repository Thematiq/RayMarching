#ifndef RAYMARCHING_ALGEBRA_H
#define RAYMARCHING_ALGEBRA_H

#include <cfloat>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>

class Line {
private:
    Eigen::Vector3d begin;
    Eigen::Vector3d direction;
    double _dir_norm;
    double t;

public:
    Line(Eigen::Vector3d direction, Eigen::Vector3d begin)
     : begin(std::move(begin)), direction(std::move(direction)), t(0), _dir_norm(direction.norm()) {}
    void moveBy(double distance);
    [[nodiscard]] Eigen::Vector3d getVec() const;
};

#endif //RAYMARCHING_ALGEBRA_H
