#include <iostream>
#include <GLFW/glfw3.h>

#define WIDTH 1280
#define HEIGHT 720

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RayMarching", NULL, NULL);
    if (!window) {
        std::cout << "Could not open window";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
