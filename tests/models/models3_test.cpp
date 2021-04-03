#include <gtest/gtest.h>
#include "models3.h"

constexpr double epsilon = 0.000000001;

// Define almost equal
#define EXPECT_AEQ(X, Y) EXPECT_TRUE(X - Y <= epsilon)

TEST(Point3_tests, arithmetic_test) {
    auto a = Point3(1, 2, 3);
    auto b = Point3(3, 2, 1);
    EXPECT_TRUE(a == Point3(1, 2, 3));
    EXPECT_FALSE(b == Point3(1, 2, 3));
    EXPECT_EQ(Point3(4, 4, 4), a+b);
    EXPECT_EQ(Point3(-1, -2, -3), -a);
    EXPECT_EQ(Point3(-2, 0, 2), a-b);
    EXPECT_EQ(Point3(2, 0, -2), b-a);
}

TEST(Point3_tests, norm_test) {
    auto a = Point3(4, 3, 0);
    EXPECT_AEQ(a.getLargeNorm(), 25.0);
    EXPECT_AEQ(a.getNorm(), 5.0);
}