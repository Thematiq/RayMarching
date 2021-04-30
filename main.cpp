#include <iostream>
#include <GLFW/glfw3.h>
#include "Camera.h"

#define WIDTH 1280
#define HEIGHT 720

using pixel = unsigned char;

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RayMarching", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Could not open window";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    auto* buffer = new pixel[WIDTH * HEIGHT * 3];

    Sphere sphere(Point3(0, 0, 0), 1);
    sphere.setColor(BLUE);
    Camera camera = Camera(Point3(-5,0,0), Point3(0,0,0), Point3(0, 0, 1));
    std::shared_ptr<Scene> scene = camera.getScene();
    scene->pushShape(&sphere);
    buffer = camera.takePhoto();

//    for (size_t x = 0; x < WIDTH; ++x) {
//        for (size_t y = 0; y < HEIGHT; ++y) {
//            for (size_t c = 0; c < 3; ++c) {
//                buffer[3*(y * WIDTH + x) + c] = x % 256;
//            }
//        }
//    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    delete[] buffer;
    exit(EXIT_SUCCESS);
}
