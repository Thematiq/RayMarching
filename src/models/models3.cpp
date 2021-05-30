#include "models3.h"

namespace RayMarching {
    using namespace Eigen;

    double Sphere::getDist(const Vector3d &p) const {
        return (p - _pos).norm() - _radius;
    }

    double Cube::getDist(const Vector3d &p) const {
        Vector3d val = (p).cwiseAbs() - _bound;
        return (val.array() < 0).select(0, val).norm();
    }
}
