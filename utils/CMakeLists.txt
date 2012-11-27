# Utils lib.
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)
set(utils_SRC utils.cc
)

add_library(utils STATIC ${utils_SRC})
