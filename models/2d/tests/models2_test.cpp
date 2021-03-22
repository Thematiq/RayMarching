#include <gtest/gtest.h>
#include "models2.h"

constexpr double epsilon = 0.000000001;

// Define almost equal
#define EXPECT_AEQ(X, Y) EXPECT_TRUE(X - Y <= epsilon)


TEST(Point2_tests, arithmetic_test) {
    auto a = Point2(1, 2);
    auto b = Point2(2, 1);
    EXPECT_TRUE(a == Point2(1, 2));
    EXPECT_FALSE(b == Point2(1, 2));
    EXPECT_EQ(Point2(3, 3), a+b);
    EXPECT_EQ(Point2(-1, -2), -a);
    EXPECT_EQ(Point2(-1, 1), a-b);
    EXPECT_EQ(Point2(1, -1), b-a);
}

TEST(Point2_tests, norm_test) {
    auto a = Point2(3, 4);
    EXPECT_AEQ(a.getLargeNorm(), 25.0);
    EXPECT_AEQ(a.getNorm(), 5.0);
}