include_directories(${CMAKE_SOURCE_DIR}/imcore)
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")
set(gabor_SWIG fringeproc.i
)

SET_SOURCE_FILES_PROPERTIES(${gabor_SWIG} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${gabor_SWIG} PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(fringeproc python ${gabor_SWIG} ${gabor_SRC})
SWIG_LINK_LIBRARIES(fringeproc ${PYTHON_LIBRARIES} ${gabor_LIBS})