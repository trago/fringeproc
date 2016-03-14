# The imcore library
include_directories(${OpenCV_INCLUDE_DIRS})

set(imcore_SRCS
	seguidor.cc
    )

add_library(imcore SHARED ${imcore_SRCS})
target_link_libraries(imcore ${OpenCV_LIBS})
target_compile_features(imcore PRIVATE c_restrict)

# Current version OpenCV does not compiles with standard 11
set_property(TARGET imcore PROPERTY CXX_STANDARD 98)
