#ifndef RAYMARCHING_SCENE_H
#define RAYMARCHING_SCENE_H
#include <vector>
#include <string>
#include <fstream>
#include <cfloat>
#include <utility>
#include <Eigen/Dense>
#include "models3.h"

using shapeDist = std::pair<double, SDFObject*>;

class Scene {
private:
    std::vector<SDFObject*> _content;
public:
    Scene() = default;
    Scene(Shape** const &items, size_t size) : _content(items, items + size) {};
    ~Scene();
    static Scene getFromFile(const std::string& filename);
    [[nodiscard]] shapeDist signedPairFunction(Eigen::Vector3d const &p) const;
    [[nodiscard]] SDFObject* signedShapeFunction(Eigen::Vector3d const &p) const;
    [[nodiscard]] double signedDistFunction(Eigen::Vector3d const &p) const;
    [[nodiscard]] size_t getShapesSize() const { return _content.size(); }
    [[nodiscard]] SDFObject* getShape(size_t index) const { return _content[index]; };
    void pushShape(SDFObject* sh) { _content.push_back(sh); };
    void destroyShape(size_t index);
};


#endif //RAYMARCHING_SCENE_H
