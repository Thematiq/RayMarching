//
// Created by thmtt on 23/03/2021.
//

#ifndef RAYMARCHING_MODELS3_H
#define RAYMARCHING_MODELS3_H

class Point3 {
private:
    double x;
    double y;
    double z;
public:
    Point3() = default;
    Point3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
    double getLargeNorm() const;
    double getNorm() const;
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
};

Point3 operator+(const Point3 &p1, const Point3 &p2);

Point3 operator-(const Point3 &p1);

Point3 operator-(const Point3 &p1, const Point3 &p2);

bool operator==(const Point3 &lhs, const Point3 &rhs);

#endif //RAYMARCHING_MODELS3_H
