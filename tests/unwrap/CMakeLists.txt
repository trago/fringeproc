set(unwrap_SRC main.cc
    unwrap_gears.c)
set(unwrap_LIBS imcore utils ${OpenCV_LIBS})

add_executable(unwrap ${unwrap_SRC})
target_link_libraries(unwrap ${unwrap_LIBS})
