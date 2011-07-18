FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(OpenCV REQUIRED)

# Para compilar cython
find_program(CYTHON_EXECUTABLE cython)

set(IMCORE_INCLUDE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/../imcore)
set(FILTERS_INCLUDE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/../filters)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${IMCORE_INCLUDE_PATH})
include_directories(${FILTERS_INCLUDE_PATH})

# Generamos los fuentes con cython
# Fuentes para crear el modulo
set(exception_SRCS
    exception.cc)
add_custom_command(OUTPUT exception.cc
    COMMAND ${CYTHON_EXECUTABLE} --cplus --fast-fail -o exception.cc -I"${IMCORE_INCLUDE_PATH}" 
    "${CMAKE_CURRENT_SOURCE_DIR}/exception.pyx")
list(APPEND ADDITIONAL_MAKE_CLEAN_FILES ${exception_SRCS})

set(filters_SRCS
    filters.cc)
add_custom_command(OUTPUT filters.cc
    COMMAND ${CYTHON_EXECUTABLE} --cplus --fast-fail -o filters.cc -I"${FILTERS_INCLUDE_PATH}" 
    "${CMAKE_CURRENT_SOURCE_DIR}/filters.pyx")
list(APPEND ADDITIONAL_MAKE_CLEAN_FILES ${filters_SRCS})


add_library(exception MODULE ${exception_SRCS})
set_target_properties(exception PROPERTIES PREFIX "")
target_link_libraries(exception imcore ${OpenCV_LIBS} ${PYTHON_LIBRARIES})

add_library(filters MODULE ${filters_SRCS})
set_target_properties(filters PROPERTIES PREFIX "")
target_link_libraries(filters cfilters ${OpenCV_LIBS} ${PYTHON_LIBRARIES})


