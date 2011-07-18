cdef extern from "fringeproc.h":
    bint filter_sgaussian(float* f, float *g, float sigma, int nr, int nc)
    bint filter_dgaussian(double *f, double *g, double sigma, int nr, int nc)
    bint filter_sreg1(float *f, float *g, float lamb, float* max_error, float* max_iters, int nr, int nc)
    bint filter_dreg1(double *f, double *g, double lamb, double* max_error, double* max_iters, int nr, int nc)
    bint filter_sreg2(float *f, float *g, float lamb, float* max_error, float* max_iters, int nr, int nc)
    bint filter_dreg2(double *f, double *g, double lamb, double* max_error, double* max_iters, int nr, int nc)
