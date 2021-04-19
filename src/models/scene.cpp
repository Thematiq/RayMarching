#include "scene.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Scene::~Scene() {
    for(auto obj : _content) {
        delete obj;
    }
}

Scene Scene::getFromFile(const std::string& filename) {
    std::ifstream file(filename);
    json data;
    Scene ret;
    file >> data;
    for (auto &arr : data["Shapes"]) {
        Shape *temp = nullptr;
        if (std::string(arr["Type"]) == "Sphere") {
            temp = new Sphere(
                    Point3(
                            (float) arr["Pos"]["x"],
                            (float) arr["Pos"]["y"],
                            (float) arr["Pos"]["z"]),
                    (float) arr["Radius"]);
        }

        ret.pushShape(temp);
    }
    return ret;
}

double Scene::signedDistFunction(const Point3 &p) const {
    auto minVal = DBL_MAX;
    for (const auto& obj : _content) {
        minVal = std::min(obj->getDist(p), minVal);
    }
    return minVal;
}

void Scene::destroyShape(size_t index) {
    Shape* sh = _content[index];
    _content.erase(_content.begin() + (long)index);
    delete sh;
}