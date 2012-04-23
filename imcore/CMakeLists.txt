# The imcore library
include_directories(${OpenCV_INCLUDE_DIRS})

set(imcore_SRCS
	seguidor.cc
    )

add_library(imcore SHARED ${imcore_SRCS})
target_link_libraries(imcore ${OpenCV_LIBS})


