set(unwrap_SRC main.cc
    unwrap_gears.c)
set(unwrap_LIBS imcore utils ${OpenCV_LIBS})

add_executable(unwrap ${unwrap_SRC})
target_link_libraries(unwrap ${unwrap_LIBS})
# Current version OpenCV does not compiles with standard 11
set_property(TARGET unwrap PROPERTY CXX_STANDARD 98)
