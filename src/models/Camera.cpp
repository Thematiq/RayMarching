#include "Camera.h"


Camera::Camera(Point3 localization, Point3 direction, Point3 up, double viewAngle, int width, int height)
        : localization(localization), viewAngle(viewAngle), width(width), height(height){
    buffer = new pixel [3 * width * height];
    scene = std::make_shared<Scene>();

    forward = Vector(localization, direction).versor();
    upward = forward.perpendicular(localization, up).versor();
    right = forward.cross(upward).versor();

//    std::cout << "Vector forward: " << forward.getX() << ", " << forward.getY() << ", " << forward.getZ() << std::endl;
//    std::cout << "Vector upward: " << upward.getX() << ", " << upward.getY() << ", " << upward.getZ() << std::endl;
//    std::cout << "Vector right: " << right.getX() << ", " << right.getY() << ", " << right.getZ() << std::endl;

    rays = (Line **) malloc(sizeof(Line *) * height);
    for(int y = 0; y < height; y++){
        rays[y] = (Line *) malloc(sizeof(Line) * width);
        for(int x = 0; x < width; x++){
            rays[y][x] = generateRay(x, y);
        }
    }
}

Line Camera::generateRay(int x, int y) const {
    double angleHorizontal = (x - (double)(width - 1) / 2) * viewAngle / width;
    double angleVertical = - (y - (double)(height - 1) / 2) * viewAngle / width;
    // std::cout << "(x, y) " << x << ", " << y << " => " << angleHorizontal << ", " << angleVertical << std::endl;
    Point3 pixelPoint = localization;
    pixelPoint = forward.extend(cos(angleHorizontal * M_PI / 180) * cos(angleVertical * M_PI / 180)).movePoint(pixelPoint);
    pixelPoint = right.extend(sin(angleHorizontal * M_PI / 180) * cos(angleVertical * M_PI / 180)).movePoint(pixelPoint);
    pixelPoint = upward.extend(sin(angleVertical * M_PI / 180)).movePoint(pixelPoint);
    // std::cout << "Pixel point: " << pixelPoint.getX() << ", " << pixelPoint.getY() << ", " << pixelPoint.getZ() << std::endl;
    Vector vec = Vector(localization, pixelPoint);
    return Line(vec, localization);
}
