# The imcore library

find_package(OpenCV REQUIRED)
find_package(PythonLibs REQUIRED)

set(imcore_SRCS
	array.cc
	exception.cc
	floodfill.cc
	seguidor.cc
	linalg.c)

add_library(imcore SHARED ${imcore_SRCS})

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_PATH})
target_link_libraries(imcore ${OpenCV_LIBS})


