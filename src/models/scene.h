#ifndef RAYMARCHING_SCENE_H
#define RAYMARCHING_SCENE_H
#include <vector>
#include <string>
#include <fstream>
#include <cfloat>
#include <utility>
#include "models3.h"

using shapeDist = std::pair<double, Shape*>;

class Scene {
private:
    std::vector<Shape*> _content;
public:
    Scene() = default;
    Scene(Shape** const &items, size_t size) : _content(items, items + size) {};
    ~Scene();
    static Scene getFromFile(const std::string& filename);
    [[nodiscard]] shapeDist signedPairFunction(Point3 const &p) const;
    [[nodiscard]] Shape* signedShapeFunction(Point3 const &p) const;
    [[nodiscard]] double signedDistFunction(Point3 const &p) const;
    [[nodiscard]] size_t getShapesSize() const { return _content.size(); }
    [[nodiscard]] Shape* getShape(size_t index) const { return _content[index]; };
    void pushShape(Shape* sh) { _content.push_back(sh); };
    void destroyShape(size_t index);
};


#endif //RAYMARCHING_SCENE_H
