#ifndef RAYMARCHING_SCENE_H
#define RAYMARCHING_SCENE_H
#include <vector>
#include <string>
#include <fstream>
#include <cfloat>
#include <utility>
#include <Eigen/Dense>
#include "models3.h"

namespace RayMarching {
    using shapeDist = std::pair<double, SDFObject *>;

    /** \brief Scene containing all objects
     *
     * Scene is responsible for handling SDF request, aggregating results from all Shapes
     */
    class Scene {
    private:
        std::vector<SDFObject *> _content;
    public:
        /**
         * Default empty Scene constructor
         */
        Scene() = default;

        /**
         * Constructor generating scene from the SDFObject array
         * @param items SDFObject pointers array
         * @param size number of elements
         */
        Scene(SDFObject **const &items, size_t size) : _content(items, items + size) {};

        /**
         * Default destructor
         */
        ~Scene();

        /**
         * Creates Scene from the JSON file
         * @param filename path to the file
         * @return Scene object
         */
        [[deprecated]] static Scene getFromFile(const std::string &filename);

        /**
         * SDF function returning pair of distance and object
         * @param p Requesting point
         * @return std::pair containing SDFObject and distance
         */
        [[nodiscard]] shapeDist signedPairFunction(Eigen::Vector3d const &p) const;

        /**
         * SDF function returning closest object
         * @param p Requesting point
         * @return Pointer to the closest SDFObject
         */
        [[nodiscard]] SDFObject *signedShapeFunction(Eigen::Vector3d const &p) const;

        /**
         * SDF function returning distance
         * @param p Requesting point
         * @return distance to the closest SDFObject
         */
        [[nodiscard]] double signedDistFunction(Eigen::Vector3d const &p) const;

        /**
         * Scene container size getter
         * @return Size of the SDFObject list
         */
        [[nodiscard]] size_t getShapesSize() const { return _content.size(); }

        /**
         * SDFObject getter
         * @param index position of the SDFObject
         * @return Pointer to the SDFObject
         * @throws std::out_of_range
         */
        [[nodiscard]] SDFObject *getShape(size_t index) const { return _content[index]; };

        /**
         * Add new SDFObject to the Scene
         * @param sh pushed SDFObject
         */
        void pushShape(SDFObject *sh) { _content.push_back(sh); };

        /**
         * Remove SDFObject at a given index
         * @param index requested index
         */
        void destroyShape(size_t index);
    };
}

#endif //RAYMARCHING_SCENE_H
