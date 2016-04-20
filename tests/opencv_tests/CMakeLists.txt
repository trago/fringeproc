set(opencv_SRC main.cc)
set(opencv_LIBS ${OpenCV_LIBS})

add_executable(opencv ${opencv_SRC})
target_link_libraries(opencv ${opencv_LIBS})
set_property(TARGET opencv PROPERTY CXX_STANDARD 98)
