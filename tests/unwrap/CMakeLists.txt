set(unwrap_SRC main.cc
    )
set(unwrap_LIBS imcore utils ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_executable(unwrap ${unwrap_SRC})
target_link_libraries(unwrap ${unwrap_LIBS})
