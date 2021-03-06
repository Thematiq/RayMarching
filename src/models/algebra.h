#ifndef RAYMARCHING_ALGEBRA_H
#define RAYMARCHING_ALGEBRA_H

#include <cfloat>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>

namespace RayMarching {

    /**
     * \brief Marching Ray implementation
     */
    class Line {
    private:
        Eigen::Vector3d begin;
        Eigen::Vector3d direction;
        double _dir_norm;
        double _t;
    public:
        Line(Eigen::Vector3d direction, Eigen::Vector3d begin)
                : begin(std::move(begin)), direction(std::move(direction)), _t(0), _dir_norm(direction.norm()) {}

        void moveBy(double distance);
        void reset() { _t = 0; }
        [[nodiscard]] Eigen::Vector3d getVec() const;
        [[nodiscard]] Eigen::Vector3d getDirection() const { return direction; }
    };
}

#endif //RAYMARCHING_ALGEBRA_H
