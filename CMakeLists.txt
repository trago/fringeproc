cmake_minimum_required(VERSION 2.8)
project(fringeproc)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_BUILD_TYPE "Release")

if(UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -fPIC -DUSE_RESTRICT_POINTER -ffast-math")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DUSE_RESTRICT_POINTER -ffast-math")
endif(UNIX)

add_subdirectory(imcore)
add_subdirectory(utils)
add_subdirectory(tests)
