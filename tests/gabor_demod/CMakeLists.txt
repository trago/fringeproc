include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(gabor_SRC main.cc
    gabor_gears.cc
    scanner.cc
    demodgabor.cc
)
set(gabor_LIBS imcore utils ${OpenCV_LIBS})

add_executable(gabor ${gabor_SRC})
target_link_libraries(gabor ${gabor_LIBS})

# Python modules
SET(CMAKE_SWIG_FLAGS "")
set(gabor_SWIG gabor.i
)

SET_SOURCE_FILES_PROPERTIES(${gabor_SWIG} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${gabor_SWIG} PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(gabor python ${gabor_SWIG} ${gabor_SRC})
SWIG_LINK_LIBRARIES(gabor ${PYTHON_LIBRARIES} ${gabor_LIBS})
