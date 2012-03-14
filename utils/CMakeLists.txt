# Utils lib.
include_directories(${CMAKE_SOURCE_DIR})
set(utils_SRC utils.cc
)

add_library(utils STATIC ${utils_SRC})
