

#ifndef RAYMARCHING_MODELS2_H
#define RAYMARCHING_MODELS2_H

class Point2 {
private:
    double x;
    double y;
public:
    Point2() = default;
    Point2(double _x, double _y) : x(_x), y(_y) {};
    double getLargeNorm() const;
    double getNorm() const;
    double getX() const { return x; }
    double getY() const { return y; }
};

Point2 operator+(const Point2 &p1, const Point2 &p2);

Point2 operator-(const Point2 &p1);

Point2 operator-(const Point2 &p1, const Point2 &p2);

bool operator==(const Point2 &lhs, const Point2 &rhs);


/**
 * Abstract class describing 2D model interface
 */
class Model2 {
public:
    /**
     * Returns distance from the viewer to the object
     * @param viewer viewer's position
     * @return distance
     */
    virtual double getDist(Point2 viewer) = 0;
};

/**
 * Class describing Circle
 */
class Circle: public Model2 {
private:
    Point2 center;
    double radius;
public:
    Circle() = default;
    Circle(Point2 _pos, double _radius): center(_pos), radius(_radius) {};
    double getDist(Point2 viewer);
};


#endif //RAYMARCHING_MODELS2_H
