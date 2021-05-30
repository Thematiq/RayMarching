#include <iostream>
#include <GLFW/glfw3.h>
#include <chrono>
#include <algorithm>
#include <memory>

#include "Camera.h"
#include "const.h"
#include <Eigen/Dense>

using namespace Eigen;
using namespace RayMarching;

int main() {


    const auto loc = Vector3d(4, -3, 0);
    const auto dir = Vector3d(0, 0, 0);
    const auto up = Vector3d(0, 0, 3);
    Settings_t settings = Settings_t();
    settings.interlace = true;
    settings.width = 640;
    settings.height = 480;
    Camera camera = Camera(loc, dir, up, settings);
    std::shared_ptr<Scene> scene = camera.getScene();

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(settings.width, settings.height, "RayMarching", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Could not open window";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


//    Cube qube(Vector3d(0, 0, 0),   1, GREEN);
//    scene->pushShape(&qube);
//    Sphere sphere(Vector3d(0, 0, 0), 1, BLUE);
//    scene->pushShape(&sphere);
//    Sphere sphere1(Vector3d(0, 2, 0), 1, RED);
//    scene->pushShape(&sphere1);
    std::unique_ptr<SDFObject> q( new Cube(Vector3d(0, -1, 0), 1, RED));
    std::unique_ptr<SDFObject> s(new Sphere(Vector3d(0, -1, 0), 1.4, BLUE));
    std::unique_ptr<SDFObject> c(new Cylinder(Vector3d(0, -1, 0), 1, 1.4, GREEN));
    std::unique_ptr<SDFObject> u(new SDFCombination(std::move(q), std::move(s), SDFCombination::SDFOperation::INTERSECTION));
    SDFCombination t(std::move(u), std::move(c), SDFCombination::SDFOperation::DIFFERENCE);
    scene->pushShape(&t);

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
    glViewport(0, 0, settings.width, settings.height);

    double angle = 0;
    while (!glfwWindowShouldClose(window)) {
        buffer = camera.takePhoto();
        glDrawPixels(settings.width, settings.height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

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
