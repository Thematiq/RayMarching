#include "algebra.h"

using namespace Eigen;

void Line::moveBy(double distance) {
    _t += distance / _dir_norm;
}

Vector3d Line::getVec() const {
    return (direction * _t) + begin;
}