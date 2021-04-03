//
// Created by teteo on 03.04.2021.
//


#include <gtest/gtest.h>
#include "models3.h"
#include "Camera.h"

constexpr double epsilon = 0.000000001;

// Define almost equal
#define EXPECT_AEQ(X, Y) EXPECT_TRUE(X - Y <= epsilon)

TEST(Camera_test, arithmetic_test) {
    Camera cam = Camera(Point3(2,3,7), Point3(5,2,-1), 60, 1280, 720);
}