#include "models3.h"

using namespace Eigen;

double Sphere::getDist(const Vector3d &p) const {
    return (p - _pos).norm() - _radius;
}

double Cube::getDist(const Vector3d &p) const {
    Vector3d val = (p).cwiseAbs() - _bound;
    return (val.array() < 0).select(0, val).norm();
}

Line Shape::getReflection(Line &ray) const {
    Eigen::Vector3d hit = ray.getVec();

    Eigen::Vector3d normal = Eigen::Vector3d(
            (getDist(hit + Eigen::Vector3d(EPSILON / 2, 0, 0)) - getDist(hit - Eigen::Vector3d(EPSILON / 2, 0, 0))) / EPSILON,
            (getDist(hit + Eigen::Vector3d(0, EPSILON / 2, 0)) - getDist(hit - Eigen::Vector3d(0, EPSILON / 2, 0))) / EPSILON,
            (getDist(hit + Eigen::Vector3d(0, 0, EPSILON / 2)) - getDist(hit - Eigen::Vector3d(0, 0, EPSILON / 2))) / EPSILON
    ).normalized();

    return Line(ray.getDirection() - 2 * ray.getDirection().dot(normal) * normal, hit);
}
