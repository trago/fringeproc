cmake_minimum_required(VERSION 2.8)
project(fringeproc)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -std=gnu99  -funroll-loops -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -funroll-loops -O3")
set(CMAKE_C_FLAGS_DEBUG "-fPIC -std=gnu99 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -g")

add_subdirectory(imcore)
add_subdirectory(utils)
add_subdirectory(tests)
