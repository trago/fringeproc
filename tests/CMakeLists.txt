include_directories(${CMAKE_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})
add_subdirectory(unwrap)
add_subdirectory(gabor_demod)
