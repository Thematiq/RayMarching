#ifndef RAYMARCHING_SCENE_H
#define RAYMARCHING_SCENE_H
#include <vector>
#include <string>
#include <fstream>
#include <cfloat>
#include "models3.h"

class Scene {
private:
    std::vector<Shape*> content;
public:
    Scene() = default;
    Scene(Shape** const &items, size_t size) : content(items, items + size) {};
    ~Scene();
    static Scene getFromFile(const std::string& filename);
    [[nodiscard]] double signedDistFunction(Point3 const &p) const;
    size_t getShapesSize() const { return content.size(); }
    Shape* getShape(size_t index) const { return content[index]; };
    void pushShape(Shape* sh) { content.push_back(sh); };
    void destroyShape(size_t index);
};


#endif //RAYMARCHING_SCENE_H
