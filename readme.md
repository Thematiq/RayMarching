## Ray marching

---

## Table of contents

[Environment setup](#setup)
[Troubleshooting](#troubleshooting)

---

## Setup

### Installing dependencies

#### Linux

> sudo pacman -S glm glfw-x11 eigen

### CLion

- Go to File -> Settings -> Build, Execution, Deployment -> CMake
- Create new profile Debug
    - CMake options: -DCMAKE_BUILD_TYPE=Debug -DBUILD_UTEST=OFF
    - leave the rest of options as default

- Create new profile Test
    - CMake options: -DCMAKE_BUILD_TYPE=Debug -DBUILD_UTEST=ON
    - leave the rest of options as default

## Troubleshooting

### CMake cannot find GLFW3Config.cmake

This is caused by linux file system case sensitivity. You can simply solve that creating symlink:

> sudo ln -s <cmake-libs>/glfw3/glfw3Config.cmake <cmake-libs>/glfw3/GLFW3Config.cmake


### CMake cannot find GLMConfig.cmake

Similar situation to the GLFW3. Create following symlink:

> sudo ln -s <cmake-libs>/glm/glmConfig.cmake <cmake-libs>/glm/GLMConfig.cmake