# This is a CMake example for Python
cmake_minimum_required(VERSION 2.8)
project(fringeproc)

#if(CMAKE_COMPILER_IS_GNUCC)
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -fPIC -fmessage-length=0 -O2 -ffast-math")
#endif(CMAKE_COMPILER_IS_GNUCC)
#if(CMAKE_COMPILER_IS_GNUCXX)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fmessage-length=0 -O2 -ffast-math")
#endif(CMAKE_COMPILER_IS_GNUCXX)
# Setting verbose mode
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_C_FLAGS_RELEASE "-std=gnu99 -O3 -mfpmath=sse -msse2 -ffast-math -funroll-loops")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -mfpmath=sse -msse2 -ffast-math -funroll-loops")
set(CMAKE_C_FLAGS_DEBUG "-std=gnu99 -ggdb3 -DBZ_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DBZ_DEBUG")

find_package(OpenCV REQUIRED)

add_subdirectory(imcore)
add_subdirectory(utils)
add_subdirectory(tests)
