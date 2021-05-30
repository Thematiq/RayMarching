#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "scene.h"

using namespace Eigen;

constexpr double epsilon = 0.000000001;

// Define almost equal
#define EXPECT_AEQ(X, Y) EXPECT_TRUE(std::abs((X) - (Y)) <= epsilon)

TEST(Scene_test, default_scene) {
    auto scene = Scene();
    ASSERT_EQ(0, scene.getShapesSize());
    auto b = new Sphere(Vector3d(1, 2, 3), 4);
    scene.pushShape(b);
    ASSERT_EQ(1, scene.getShapesSize());
    ASSERT_EQ(b, scene.getShape(0));
}

TEST(Scene_test, from_array) {
    Shape* items[] = {
            new Sphere(Vector3d(1, 2, 3), 4),
            new Sphere(Vector3d(4, 5, 6), 7),
            new Sphere(Vector3d(10, 10, 10), 10)
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
    ASSERT_EQ(2, scene.getShapesSize());
    auto a = (Sphere*) scene.getShape(0);
    ASSERT_EQ(a->getPos(), Vector3d(0, 0, 0));
    ASSERT_EQ(a->getRadius(), 0.0);
}

TEST(Scene_test, destroy) {
    auto scene = Scene::getFromFile(TEST_SCENE);
    ASSERT_EQ(2, scene.getShapesSize());
    scene.destroyShape(0);
    ASSERT_EQ(1, scene.getShapesSize());
}

TEST(Scene_test, SDF) {
    auto scene = Scene::getFromFile(TEST_SCENE);
    auto p = Vector3d(-1, 0, 0);
    EXPECT_AEQ(scene.signedDistFunction(p), 1);
}