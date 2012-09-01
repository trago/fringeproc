add_subdirectory(imcore)
add_subdirectory(gui)

add_custom_target(python_fringes ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/fringes.py ${CMAKE_CURRENT_BINARY_DIR})
