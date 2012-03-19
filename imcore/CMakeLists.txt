# The imcore library
include_directories(${OpenCV_INCLUDE_DIRS})

set(imcore_SRCS
	array.cc
	exception.cc
	floodfill.cc
	seguidor.cc
	linalg.c)

add_library(imcore SHARED ${imcore_SRCS})
target_link_libraries(imcore ${OpenCV_LIBS})


