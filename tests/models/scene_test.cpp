#include <gtest/gtest.h>
#include "scene.h"

TEST(Scene_test, default_scene) {
    auto scene = Scene();
    ASSERT_EQ(0, scene.getShapesSize());
    auto b = new Sphere(Point3(1, 2, 3), 4);
    scene.pushShape(b);
    ASSERT_EQ(1, scene.getShapesSize());
    ASSERT_EQ(b, scene.getShape(0));
}

TEST(Scene_test, from_array) {
    Shape* items[] = {
            new Sphere(Point3(1, 2, 3), 4),
            new Sphere(Point3(4, 5, 6), 7),
            new Sphere(Point3(10, 10, 10), 10)
    };
    size_t size = 3;

    auto scene = Scene(items, size);
    ASSERT_EQ(size, scene.getShapesSize());
    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(items[i], scene.getShape(i));
    }
}

TEST(Scene_test, from_file) {
    auto scene = Scene::getFromFile(TEST_SCENE);
    ASSERT_EQ(1, scene.getShapesSize());
    auto a = (Sphere*) scene.getShape(0);
    ASSERT_EQ(a->getPos(), Point3(0, 0, 0));
    ASSERT_EQ(a->getRadius(), 0.0);
}