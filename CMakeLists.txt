# Minimum required CMake version
cmake_minimum_required(VERSION 3.30)

# Project Name and version
project(VSC-CMAKE-CXX-TEMPLATE VERSION 1.0.0)

# Common utility functions:
include(${PROJECT_SOURCE_DIR}/cmake/utils/common.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# @see "./cmake/utils/common.cmake"
set_default_values(
    # CMake project namespace:
    # All the libs should have a alias with this namespace
    PROJECT_NAMESPACE "GRAFIX::"
)

# Utility functions for installing libraries:
include(${PROJECT_SOURCE_DIR}/cmake/utils/lib-install.cmake)

# Compiler configurations for cpp, e.g., g++, clang++, msvc(cl), etc.:
include(${PROJECT_SOURCE_DIR}/cmake/compilers/cxx-compiler-configs.cmake)

# Find Vulkan libraries:
include(${PROJECT_SOURCE_DIR}/cmake/libraries/libvulkan.cmake)

# Other libraries managed by VCPKG
find_package(cxxopts CONFIG REQUIRED)
find_package(EnTT    CONFIG REQUIRED)
find_package(glfw3   CONFIG REQUIRED)
find_package(imgui   CONFIG REQUIRED)
find_package(spdlog  CONFIG REQUIRED)
find_package(proxy   CONFIG REQUIRED)
find_package(stdexec CONFIG REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples)