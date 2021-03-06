#include "scene.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace RayMarching {

    using namespace Eigen;
    using json = nlohmann::json;

    Scene::~Scene() {
        for (auto obj : _content) {
            delete obj;
        }
    }

    Scene Scene::getFromFile(const std::string &filename) {
        std::ifstream file(filename);
        json data;
        Scene ret;
        file >> data;
        for (auto &arr : data["Shapes"]) {
            Shape *temp = nullptr;
            if (std::string(arr["Type"]) == "Sphere") {
                temp = new Sphere(
                        Vector3d(
                                (float) arr["Pos"]["x"],
                                (float) arr["Pos"]["y"],
                                (float) arr["Pos"]["z"]),
                        (float) arr["Radius"]);
            }

            ret.pushShape(temp);
        }
        return ret;
    }

    shapeDist Scene::signedPairFunction(const Vector3d &p) const {
        shapeDist ret = {DBL_MAX, nullptr};
        for (auto &member : _content) {
            double dst = member->getDist(p);
            if (dst < ret.first) {
                ret.first = dst;
                ret.second = member;
            }
        }
        return ret;
    }

    SDFObject *Scene::signedShapeFunction(const Vector3d &p) const {
        return signedPairFunction(p).second;
    }

    double Scene::signedDistFunction(const Vector3d &p) const {
        return signedPairFunction(p).first;
    }

    void Scene::destroyShape(size_t index) {
        SDFObject *sh = _content[index];
        _content.erase(_content.begin() + (long) index);
        delete sh;
    }
}