cmake_minimum_required(VERSION 3.17)
project(RayMarching)

set(CMAKE_CXX_STANDARD 17)


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

if (WIN32)
    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
elseif (UNIX)

endif()

# ===================================================================
# GTEST cmake macro
# ===================================================================

function(add_gmock_test target main_dir)
    set(CXX_HEADERS ${main_dir}/inc)
    file(GLOB CXX_SRC "${main_dir}/src/*.cpp")
    file(GLOB CXX_TEST "${main_dir}/tests/*.cpp")
    add_executable(${target} ${CXX_TEST} ${CXX_SRC})
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    target_include_directories(${target} PUBLIC ${CXX_HEADERS})
    target_link_libraries(${target} PUBLIC gtest_main)

    add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND ${target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running ${target}" VERBATIM
    )
    add_test(${target} ${target})
endfunction()


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
    option(EXAMPLE_TEST "Run example unit test" ON)

    if (${EXAMPLE_TEST})
        add_subdirectory(example)
    endif()

else()
    # ===================================================================
    # Application build
    # ===================================================================
    message("===== RUNNING MAIN APP BUILD =====")

    add_executable(RayMarching
            main.cpp
            )
endif()
