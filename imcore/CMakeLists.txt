# The imcore library
include_directories(${CMAKE_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/include/Eigen3)

set(imcore_SRCS
  demodgabor.cc
  gabor_gears.cc
  scanner.cc
  unwrap_gears.c
  unwrap.cc
  )

add_library(imcore STATIC ${imcore_SRCS})
target_link_libraries(imcore ${OpenCV_LIBS})


