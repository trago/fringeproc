set(gabor_SRC main.cc
    gabor_gears.cc
    scanner.cc
    demodgabor.cc
)
set(gabor_LIBS imcore utils ${OpenCV_LIBS})

add_executable(gabor ${gabor_SRC})
target_link_libraries(gabor ${gabor_LIBS})
