#include <iostream>
#include <GLFW/glfw3.h>
#include <chrono>
#include <algorithm>

#include "Camera.h"
#include "const.h"


using pixel = unsigned char;

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RayMarching", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Could not open window";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    const Point3 loc = Point3(-10, 0, 0);
    const Point3 dir = Point3(0, 0, 0);
    const Point3 up = Point3(0, 0, 1);
    Camera camera = Camera(loc, dir, up, true);
    std::shared_ptr<Scene> scene = camera.getScene();


//    Cube q(Point3(0, -1, 0), Point3(1, 1, 1));
//    scene->pushShape(&q);
    Sphere sphere1(Point3(0, -1, 0), 1);
    Sphere sphere2(Point3(0, 1, 0), 1);
    sphere1.setColor(BLUE);
    scene->pushShape(&sphere1);
    scene->pushShape(&sphere2);

    std::cout << "Context prepared" << std::endl;

    constexpr unsigned int TRIALS = 1;
    unsigned int times[TRIALS];
    pixel* buffer;

    for (unsigned int & time : times) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        buffer = camera.takePhoto();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
    std::cout << "RENDERD STATISTICS FOR " << TRIALS << " TRIALS" << std::endl;
    std::cout << "Min time (ms) " << *std::min_element(times, times+TRIALS) << std::endl;
    std::cout << "Max time (ms) " << *std::max_element(times, times+TRIALS) << std::endl;
    unsigned int total = 0;
    for (auto && time : times) {
        total += time;
    }
    std::cout << "Avg time (ms) " << total / TRIALS << std::endl;


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
