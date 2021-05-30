#include "models3.h"

namespace RayMarching {
    using namespace Eigen;

    Line SDFObject::getReflection(const Line &ray) const {
        Eigen::Vector3d hit = ray.getVec();

        Eigen::Vector3d normal = Eigen::Vector3d(
                (getDist(hit + Eigen::Vector3d(EPSILON / 2, 0, 0)) -
                 getDist(hit - Eigen::Vector3d(EPSILON / 2, 0, 0))) / EPSILON,
                (getDist(hit + Eigen::Vector3d(0, EPSILON / 2, 0)) -
                 getDist(hit - Eigen::Vector3d(0, EPSILON / 2, 0))) / EPSILON,
                (getDist(hit + Eigen::Vector3d(0, 0, EPSILON / 2)) -
                 getDist(hit - Eigen::Vector3d(0, 0, EPSILON / 2))) / EPSILON
        ).normalized();

        return Line(ray.getDirection() - 2 * ray.getDirection().dot(normal) * normal, hit);
    }

    std::pair<double, double> SDFCombination::getSoloDist(const Vector3d &p) const {
        return std::pair<double, double>(
            _left->getDist(p),
            _right->getDist(p)
        );
    }

    double SDFCombination::getDist(const Vector3d &p) const {
        std::pair<double, double> dists = getSoloDist(p);
        switch (_op) {
            case SDFOperation::UNION:
                return std::min(dists.first, dists.second);
            case SDFOperation::INTERSECTION:
                return std::max(dists.first, dists.second);
            case SDFOperation::DIFFERENCE:
                return std::max(dists.first, -dists.second);
        }
    }

    color_t SDFCombination::getColor(const Vector3d &p) const {
        std::pair<double, double> dists = getSoloDist(p);
        if (dists.first < dists.second) {
            return _left->getColor(p);
        } else {
            return _right->getColor(p);
        }
    }

    double Sphere::getDist(const Vector3d &p) const {
        return (p - _pos).norm() - _radius;
    }

    double Cube::getDist(const Vector3d &p) const {
        Vector3d val = (p - _pos).cwiseAbs() - _bound;
        return (val.array() < 0).select(0, val).norm();
    }

    double Cylinder::getDist(const Vector3d &p) const {
        Vector3d pos = p - _pos;
        Vector2d d = Vector2d(Vector2d(pos.x(), pos.z()).norm(), pos.y()) - _hr;
        return std::min(std::max(d.x(), d.y()), 0.0) + (d.array() < 0).select(0, d).norm();
    }
}
