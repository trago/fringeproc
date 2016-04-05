# This is a CMake example for Python
cmake_minimum_required(VERSION 2.8)
project(fringeproc)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
                              ${CMAKE_SOURCE_DIR}/CMake/common)
include(GitExternal)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -O2 -mfpmath=sse -msse2 -funroll-loops")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -mfpmath=sse -msse2 -funroll-loops")
set(CMAKE_C_FLAGS_DEBUG "-fPIC -ggdb3 -DBZ_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -ggdb3 -DBZ_DEBUG")

#include(Common)

# Look for required libraries and tools
find_package(OpenCV REQUIRED)
find_package(SWIG REQUIRED)
find_package(PythonLibs)
find_package(NumPy)
include(${SWIG_USE_FILE})
add_subdirectory(imcore)
add_subdirectory(utils)
add_subdirectory(tests)
