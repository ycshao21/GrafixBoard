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
    # @note Change this to your own namespace
    PROJECT_NAMESPACE "_template_project_name_::"
)

# Compiler configurations for cpp, e.g., g++, clang++, msvc(cl), etc.
include(${PROJECT_SOURCE_DIR}/cmake/compilers/cxx-compiler-configs.cmake)
# Compiler configurations for CUDA 
include(${PROJECT_SOURCE_DIR}/cmake/compilers/cuda-compiler-configs.cmake)
# Some preparational steps for linking CUDA libraries:
include(${PROJECT_SOURCE_DIR}/cmake/libraries/libcuda.cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)