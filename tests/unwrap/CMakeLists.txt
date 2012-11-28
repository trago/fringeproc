find_package(X11)

include_directories(${X11_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)

set(unwrap_SRC main.cc
    )
set(unwrap_LIBS imcore utils)

add_executable(unwrap ${unwrap_SRC})
target_link_libraries(unwrap ${unwrap_LIBS} ${X11_LIBRARIES})
