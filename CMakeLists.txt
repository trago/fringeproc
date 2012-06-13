# This is a CMake example for Python
cmake_minimum_required(VERSION 2.8)
project(fringeproc)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -std=gnu99 -O2 -mfpmath=sse -msse2 -funroll-loops")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -mfpmath=sse -msse2 -funroll-loops")
set(CMAKE_C_FLAGS_DEBUG "-fPIC -std=gnu99 -ggdb3 -DBZ_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -ggdb3 -DBZ_DEBUG")

# Look for required libraries and tools
find_package(OpenCV REQUIRED)
find_package(SWIG REQUIRED)
find_package(PythonLibs)
include(${SWIG_USE_FILE})

add_subdirectory(imcore)
add_subdirectory(utils)
add_subdirectory(tests)
add_subdirectory(python)
