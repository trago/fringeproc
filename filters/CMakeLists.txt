# The filters library

set(filters_SRCS
	convolution.c
    demodula.c
    document.c
    follower.cc
    fringeproc.c
    matrix.cc
    mfuntions.c
    optimization.c
    seguidor.cc)

add_library(cfilters SHARED ${filters_SRCS})
target_link_libraries(cfilters -lblas)

