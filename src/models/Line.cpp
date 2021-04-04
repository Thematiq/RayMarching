

#include <cmath>
#include "Line.h"


void Line::moveBy(double distance) {
    this->t += distance / direction.getLength();
}

Point3 Line::getPoint3() {
    std::cout << direction.getX();
    Vector a = direction.extend(t);
    std::cout << " | " << a.getX() << std::endl;
    return direction.extend(t).movePoint(begin);
}





