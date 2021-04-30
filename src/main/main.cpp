#include <iostream>
#include <GLFW/glfw3.h>

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
    auto* buffer = camera.takePhoto();

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
