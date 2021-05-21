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

    Sphere sphere1(Point3(0, -1, 0), 1);
    Sphere sphere2(Point3(0, 1, 0), 1);
    sphere1.setColor(BLUE);

    Camera camera = Camera(Point3(-5,0,0), Point3(0,0,0), Point3(0, 0, 1));
    std::shared_ptr<Scene> scene = camera.getScene();
    scene->pushShape(&sphere1);
    scene->pushShape(&sphere2);

    std::cout << "Context prepared" << std::endl;

    constexpr unsigned int TRIALS = 10;
    unsigned int times[TRIALS];
    pixel* buffer;

    for (int i = 0; i < TRIALS - 1; ++i) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        buffer = camera.takePhoto();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        times[i] = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
    std::cout << "RENDERD STATISTICS FOR " << TRIALS << " TRIALS" << std::endl;
    std::cout << "Min time (ms) " << *std::min_element(times, times+TRIALS-1) << std::endl;
    std::cout << "Max time (ms) " << *std::max_element(times, times+TRIALS-1) << std::endl;
    unsigned int total = 0;
    for (auto && time : times) {
        total += time;
    }
    std::cout << "Avg time (ms) " << total / TRIALS << std::endl;


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
