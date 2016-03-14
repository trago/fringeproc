# Utils lib.
include_directories(${CMAKE_SOURCE_DIR})
set(utils_SRC utils.cc
)

add_library(utils STATIC ${utils_SRC})
target_link_libraries(utils ${OpenCV_LIBS})
# Current version OpenCV does not compiles with standard 11
set_property(TARGET utils PROPERTY CXX_STANDARD 98)
