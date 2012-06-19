# The imcore library
include_directories(${OpenCV_INCLUDE_DIRS})

set(imcore_SRCS
  seguidor.cc
  demodgabor.cc
  gabor_gears.cc
  scanner.cc
  unwrap_gears.c
  unwrap.cc
  )

add_library(imcore SHARED ${imcore_SRCS})
target_link_libraries(imcore ${OpenCV_LIBS})


