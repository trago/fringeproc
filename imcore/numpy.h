#ifndef NUMPY_IMPORT_H
#define NUMPY_IMPORT_H

#include <Python.h>
#include <numpy/arrayobject.h>
#include "array.h"

/**
 * Initializes the numpy and cv modules.
 * 
 */
void initialize_numpy();

/**
 * Converts a Numpy array to an Array data.
 */
bool NumpyArray2Array(PyObject* in, Adata** data);

/**
 * Says if the python object is a numpy array.
 */
bool is_numpy_array(PyObject* in);

/**
 * Converts an Array data to a Numpy array.
 */
bool Array2NumpyArray(Adata* data, PyObject** out);

bool Array2NumpyArray_copy(Adata* data, PyObject** out);

void delete_array(Adata* data);

#endif