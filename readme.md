## Ray marching

---

## Table of contents

[Environment setup](#setup)

---

## Setup

### CLion

- Go to File -> Settings -> Build, Execution, Deployment -> CMake
- Create new profile Debug
    - CMake options: -DCMAKE_BUILD_TYPE=Debug -DBUILD_UTEST=OFF
    - leave the rest of options as default

- Create new profile Test
    - CMake options: -DCMAKE_BUILD_TYPE=Debug -DBUILD_UTEST=ON
    - leave the rest of options as default
