#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H
#include <cfloat>
#include <cmath>
#include <memory>
#include <Eigen/Dense>
#include <utility>
#include "const.h"
#include "colors.h"
#include "algebra.h"

namespace RayMarching {

    /**
     * \brief Virtual class describing abstract shapes
     *
     * Interface defining required behavior for the Shape implementation
     */
    class SDFObject {
    public:
        /**
         * Virtual destructor for the inheritance safety
         */
        virtual ~SDFObject() = default;

        /**
         * SDF function for the SDFObject
         * @param p Requesting point
         * @return signed distance from the object
         */
        [[nodiscard]] virtual double getDist(Eigen::Vector3d const &p) const = 0;

        /**
         * Returns closest color from the given position
         * @param p Requesting point
         * @return color_t structure
         */
        [[nodiscard]] virtual color_t getColor(Eigen::Vector3d const &p) const = 0;

        /**
         * Returns reflected ray from the object
         * @param ray Line pointing from the object
         * @return Reflected Line
         */
        [[nodiscard]] Line getReflection(const Line& ray) const;
    };

    /**
     * \brief SDFObject combining two elements
     */
    class SDFCombination : public SDFObject {
    public:
        /**
         * \brief Available SDFObject combination
         */
        enum class SDFOperation {
            UNION,              /**< Creates object behaving as union of two SDFObject */
            INTERSECTION,       /**< Creates object behaving as intersection of two SDFObject */
            DIFFERENCE          /**< Creates difference _left / _right of two SDFObject */
        };
    private:
        const std::unique_ptr<SDFObject> _left;
        const std::unique_ptr<SDFObject> _right;
        const SDFOperation _op;
    protected:
        /**
         * Returns pair of distances from objects
         * @param p Requesting point
         * @return pair of distances
         */
        [[nodiscard]] std::pair<double, double> getSoloDist(const Eigen::Vector3d &p) const;
    public:
        /**
         * Creates new combination
         * @param left unique pointer to the SDFObject
         * @param right unique pointer to the SDFObject
         * @param op type of operation
         */
        SDFCombination(std::unique_ptr<SDFObject> left, std::unique_ptr<SDFObject> right, SDFOperation op)
            : _left(std::move(left)), _right(std::move(right)), _op(op) {}

        [[nodiscard]] double getDist(const Eigen::Vector3d &p) const override;
        [[nodiscard]] color_t getColor(const Eigen::Vector3d &p) const override;
    };

    class Shape : public SDFObject {
    protected:
        Eigen::Vector3d _pos;
        color_t _color;
    public:
        explicit Shape(Eigen::Vector3d p, color_t color = BLACK) : _pos(std::move(p)), _color(color) {};
        [[nodiscard]] virtual Eigen::Vector3d getPos() const { return _pos; };
        [[nodiscard]] color_t getColor(const Eigen::Vector3d &p) const override { return _color; }
        void setColor(color_t color){ _color = color; }
    };



    class Sphere : public Shape {
    private:
        double _radius;
    public:
        explicit Sphere(Eigen::Vector3d p, double r = 0, color_t color = BLACK) : Shape(std::move(p), color), _radius(r) {};

        [[nodiscard]] double getDist(const Eigen::Vector3d &p) const override;

        [[nodiscard]] double getRadius() const { return _radius; }
    };

    class Cube : public Shape {
    private:
        Eigen::Vector3d _bound;
    public:
        /**
         * Creates new Cube
         * @param p center of the Cube
         * @param bound side length of the Cube
         */
        Cube(Eigen::Vector3d p, double bound, color_t color = BLACK) : Shape(std::move(p), color), _bound(bound, bound, bound) {};

        [[nodiscard]] double getDist(const Eigen::Vector3d &p) const override;

        [[nodiscard]] double getBound() const { return _bound.x(); }
    };

    class Cylinder : public Shape {
    private:
        Eigen::Vector2d _hr;
    public:
        /**
         * Creates new Cylinder
         * @param p center of the Cylinder
         * @param h height of the Cylinder
         * @param r radius of the Cylinder
         */
        Cylinder(Eigen::Vector3d p, double h, double r, color_t color = BLACK) : Shape(std::move(p), color), _hr(h, r) {};

        [[nodiscard]] double getDist(const Eigen::Vector3d &p) const override;
    };
}

#endif //RAYMARCHING_MODELS3_H
