#include <iostream>
#include <GLFW/glfw3.h>
#include <chrono>
#include <algorithm>

#include "Camera.h"
#include "const.h"
#include <Eigen/Dense>

using namespace Eigen;
using namespace RayMarching;

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RayMarching", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Could not open window";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    const auto loc = Vector3d(4, -3, 0);
    const auto dir = Vector3d(0, 0, 0);
    const auto up = Vector3d(0, 0, 3);
    Settings_t settings = Settings_t();
    settings.interlace = false;
    Camera camera = Camera(loc, dir, up, settings);
    std::shared_ptr<Scene> scene = camera.getScene();


    Cube qube(Vector3d(0, -1, 0),   1);
    qube.setColor(GREEN);
    scene->pushShape(&qube);
    Sphere sphere(Vector3d(0, -2, 0), 1);
    sphere.setColor(BLUE);
    scene->pushShape(&sphere);
    Sphere sphere1(Vector3d(0, 2, 0), 1);
    sphere1.setColor(RED);
    scene->pushShape(&sphere1);

    std::cout << "Context prepared" << std::endl;

    constexpr unsigned int TRIALS = 16;
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

    double angle = 0;
    while (!glfwWindowShouldClose(window)) {
        buffer = camera.takePhoto();
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);

        glfwPollEvents();
        glfwSwapBuffers(window);

        camera.setCamera(Vector3d(4 * cos(angle), 4 * sin(angle), 0),
                         Vector3d(0, 0, 0),
                         Vector3d(0, 0, 3));
        angle += 0.01;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    delete[] buffer;
    exit(EXIT_SUCCESS);
}
