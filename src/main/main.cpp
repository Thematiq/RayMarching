#include <iostream>
#include <GLFW/glfw3.h>
#include <chrono>
#include <algorithm>

#include "Camera.h"
#include "const.h"
#include <Eigen/Dense>

using namespace Eigen;
using namespace RayMarching;
using pixel = unsigned char;

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RayMarching", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Could not open window";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    const auto loc = Vector3d(-10, 0, 0);
    const auto dir = Vector3d(0, 0, 0);
    const auto up = Vector3d(0, 0, 1);
    Camera camera = Camera(loc, dir, up, true);
    std::shared_ptr<Scene> scene = camera.getScene();


    Cube q(Vector3d(0, -1, 0), 1);
    scene->pushShape(&q);
//    Sphere sphere1(Vector3d(0, -1, 0), 1);
//    Sphere sphere2(Vector3d(0, 1, 0), 1);
//    sphere1.setColor(BLUE);
//    scene->pushShape(&sphere1);
//    scene->pushShape(&sphere2);

    std::cout << "Context prepared" << std::endl;

    constexpr unsigned int TRIALS = 10;
    unsigned int times[TRIALS];
    pixel* buffer;

    for (unsigned int & time : times) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        buffer = camera.takePhoto();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
    std::cout << "RENDER STATISTICS FOR " << TRIALS << " TRIALS" << std::endl;
    std::cout << "Min time (ms) " << *std::min_element(times, times+TRIALS) << std::endl;
    std::cout << "Max time (ms) " << *std::max_element(times, times+TRIALS) << std::endl;
    unsigned int total = 0;
    for (auto && time : times) {
        total += time;
    }
    std::cout << "Avg time (ms) " << total / TRIALS << std::endl;
    std::cout << "Avg FPS " << (1000.0f * TRIALS) / total << std::endl;


    glfwMakeContextCurrent(window);
    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        buffer = camera.takePhoto();
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    delete[] buffer;
    exit(EXIT_SUCCESS);
}
