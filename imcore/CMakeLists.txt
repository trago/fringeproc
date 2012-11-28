# The imcore library
include_directories(${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include)

set(imcore_SRCS
  demodgabor.cc
  gabor_gears.cc
  scanner.cc
  unwrap_gears.c
  unwrap.cc
  )

add_library(imcore STATIC ${imcore_SRCS})
target_link_libraries(imcore)

