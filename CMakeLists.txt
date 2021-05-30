cmake_minimum_required(VERSION 3.17)
project(RayMarching)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O3")

# ===================================================================
# Gtest unit tests setup
# ===================================================================

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if (WIN32)
    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
elseif (UNIX)

endif()

# Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# ===================================================================
# GTEST cmake macro
# ===================================================================

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)


function(add_gmock_test target module)
    set(CXX_HEADERS ${SOURCE_DIR}/${module})
    file(GLOB CXX_SRC "${SOURCE_DIR}/${module}/*.cpp")
    file(GLOB CXX_TEST "${TEST_DIR}/${module}/*.cpp")
    add_executable(${target} ${CXX_TEST} ${CXX_SRC})
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    target_include_directories(${target} PUBLIC ${CXX_HEADERS})
    target_link_libraries(${target} PUBLIC gtest_main)
    cmake_parse_arguments(TARGET
            ""
            "ENV"
            "LINK"
            ${ARGN}
            )
    if (NOT DEFINED TARGET_ENV)
        set(TARGET_ENV "")
    endif()

    message("==================================")
    message("TEST ${target}")
    message("ENV ${TARGET_ENV}")
    message("LIBS ${TARGET_LINK}")
    message("==================================")

    target_compile_definitions(${target} PRIVATE ${TARGET_ENV})

    add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND ${target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running ${target}" VERBATIM
    )

    if (DEFINED TARGET_LINK)
        target_link_libraries(
                ${target} PRIVATE
                ${TARGET_LINK}
        )
    endif()

    add_test(${target} ${target})
endfunction()

# ===================================================================
# OpenGL
# ===================================================================

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)

# ===================================================================
# Main build options
# ===================================================================

option(BUILD_UTEST "Build all unit tests" ON)

if (${BUILD_UTEST})
    # ===================================================================
    # Unit tests
    # ===================================================================
    message("===== RUNNING UNIT TESTS BUILD =====")
    # Options
    option(MODELS_TEST "Run 2D models unit tests" ON)

    if (${MODELS_TEST})
        message("Setting up models tests")
        add_subdirectory(tests/models)
    endif()

else()
    # ===================================================================
    # Application build
    # ===================================================================
    message("===== RUNNING MAIN APP BUILD =====")

    # Add models directory to main build
    set(MODELS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/models")
    file(GLOB MODELS_CXX "${CMAKE_CURRENT_SOURCE_DIR}/src/models/*.cpp")
    file(GLOB MODELS_INC "${CMAKE_CURRENT_SOURCE_DIR}/src/models/*.h")

    set(MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/main")
    file(GLOB MAIN_CXX  "${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.cpp")
    file(GLOB MAIN_INC "${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.h")

    set(TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tools")
    file(GLOB TOOLS_CXX "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp")
    file(GLOB TOOLS_INC "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.h")

    add_executable(RayMarching
            ${MAIN_CXX} ${MAIN_INC}
            ${MODELS_CXX} ${MODELS_INC}
            ${TOOLS_CXX} ${TOOLS_INC}
            )

    include_directories(
            ${OPENGL_INCLUDE_DIRS}
            ${GLFW_INCLUDE_DIRS}
    )

    target_include_directories(
            RayMarching PUBLIC
            ${MODELS_DIR}
            ${MAIN_DIR}
            ${TOOLS_DIR}
    )

    target_link_libraries( RayMarching
            OpenGL::GL
            nlohmann_json::nlohmann_json
            glfw
            Threads::Threads
            Eigen3::Eigen
    )

endif()







