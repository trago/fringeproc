if(UNIX)
  find_package(X11)
  set(OTHER_LIBRARIES ${X11_LIBRARIES} -lpthread)
elseif(WIN32)
  set(OTHER_LIBRARIES )
endif(UNIX)

include_directories(${X11_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include)

set(unwrap_SRC main.cc c_unwrap.cc
    )
set(unwrap_LIBS imcore utils)

add_executable(unwrap ${unwrap_SRC})
target_link_libraries(unwrap ${unwrap_LIBS} ${OTHER_LIBRARIES})
