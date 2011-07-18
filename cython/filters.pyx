from __future__ import division
import numpy as np
cimport numpy as np

def gaussian(np.ndarray g, float sigma):
    if g.ndim != 2:
        raise ValueError("Only two dimensional arrays are allowed")
    assert g.dtype == np.float64 or g.dtype == np.float32
    
    cdef int nr = g.shape[0]
    cdef int nc = g.shape[1]
    cdef void* dat
    cdef np.ndarray f
    
    if g.dtype == np.float32:
        f = np.zeros([nr,nc], dtype=np.float32)
        dat = f.data
        filter_sgaussian(<float*>dat, <float*>g.data, sigma, nr, nc)
    else:
        f = np.zeros([nr,nc], dtype=np.float64)
        dat = f.data
        filter_dgaussian(<double*>dat, <double*>g.data, sigma, nr, nc)
        
    return f

def reg_1order(np.ndarray g, float lamb, float error_TOL, float iters):
    if g.ndim != 2:
        raise ValueError("Only two dimensional arrays are allowed")
    assert g.dtype == np.float64 or g.dtype == np.float32
    
    cdef int nr = g.shape[0]
    cdef int nc = g.shape[1]
    cdef void* dat
    cdef np.ndarray f
    cdef double max_iters
    cdef double max_error
    
    if g.dtype == np.float32:
        f = np.zeros([nr,nc], dtype=np.float32)
        dat = f.data
        filter_sreg1(<float*> dat, <float*>g.data, lamb, 
            &error_TOL, &iters, nr, nc)
    else:
        max_iters = iters
        max_error = error_TOL
        f = np.zeros([nr,nc], dtype=np.float64)
        dat = f.data
        filter_dreg1(<double*> dat, <double*>g.data, lamb, 
            &max_error, &max_iters, nr, nc)
        iters = max_iters
        error_TOL = max_error
        
    return f, iters, error_TOL

def reg_2order(np.ndarray g, float lamb, float error_TOL, float iters):
    if g.ndim != 2:
        raise ValueError("Only two dimensional arrays are allowed")
    assert g.dtype == np.float64 or g.dtype == np.float32
    
    cdef int nr = g.shape[0]
    cdef int nc = g.shape[1]
    cdef void* dat
    cdef np.ndarray f
    cdef double max_iters
    cdef double max_error
    
    if g.dtype == np.float32:
        f = np.zeros([nr,nc], dtype=np.float32)
        dat = f.data
        filter_sreg2(<float*> dat, <float*>g.data, lamb, 
            &error_TOL, &iters, nr, nc)
    else:
        max_iters = iters
        max_error = error_TOL
        f = np.zeros([nr,nc], dtype=np.float64)
        dat = f.data
        filter_dreg2(<double*> dat, <double*>g.data, lamb, 
            &max_error, &max_iters, nr, nc)
        iters = max_iters
        error_TOL = max_error
        
    return f, iters, error_TOL
