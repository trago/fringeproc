include_directories(${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include)

set(gabor_SRC main.cc
)
set(gabor_LIBS imcore utils ${OpenCV_LIBS})

add_executable(gabor ${gabor_SRC})
target_link_libraries(gabor ${gabor_LIBS})
