include_directories(${CMAKE_SOURCE_DIR}/imcore)
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${NUMPY_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")
set(fringeproc_SWIG fringeproc.i
)

SET(fringeproc_LIBS imcore utils ${PYTHON_LIBRARIES})

SET_SOURCE_FILES_PROPERTIES(${fringeproc_SWIG} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${fringeproc_SWIG} PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(fringeproc python ${fringeproc_SWIG} ${fringeproc_SRC})
SWIG_LINK_LIBRARIES(fringeproc  ${fringeproc_LIBS})
