include_directories(${CMAKE_SOURCE_DIR}
)

set(seguidor_LIBS utils
    imcore
    ${OpenCV_LIBS}
)

add_executable(seguidor seguidor.cc)
target_link_libraries(seguidor ${seguidor_LIBS})
