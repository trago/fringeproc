# This is a CMake example for Python
cmake_minimum_required(VERSION 2.8)
project(fringeproc)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -std=gnu99 -O2  -funroll-loops")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -funroll-loops")
set(CMAKE_C_FLAGS_DEBUG "-fPIC -std=gnu99 -g -DBZ_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -g -DBZ_DEBUG")

add_subdirectory(imcore)
add_subdirectory(utils)
add_subdirectory(tests)
#add_subdirectory(python)
