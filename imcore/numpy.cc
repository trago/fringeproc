#include "numpy.h"

#include <Python.h>
#include <numpy/arrayobject.h>
#include <iostream>

/** A reference to the python iplimage type */
PyObject* gpy_iplimage = NULL;
/** A reference to the python cvmat type */
PyObject* gpy_cvmat = NULL;

void initialize_numpy()
{
    // Imports the numpy array python's API
    import_array();

    // Imports the OpenCv iplimage and cvmat python's types.
    PyObject* cv_module = PyImport_ImportModule("cv"); // Imports the cv module
    PyObject* cv_dict = PyModule_GetDict(cv_module);
    PyObject* strKey = PyString_FromString("iplimage");
    gpy_iplimage = PyDict_GetItem(cv_dict, strKey); // Gets the iplimage type
    Py_DecRef(strKey);
    strKey = PyString_FromString("cvmat");
    gpy_cvmat = PyDict_GetItem(cv_dict, strKey); // Gets the cvmat type
    Py_DecRef(strKey);
    Py_DecRef(cv_module);
}

int is_iplimage(PyObject* o)
{
    return PyObject_IsInstance(o, gpy_iplimage);
}

bool is_numpy_array(PyObject* in)
{
    if (!PyArray_Check(in))
        return false;

    PyArrayObject* array = (PyArrayObject*)in;
    if (array->nd != 2)
        return false;

    switch (PyArray_TYPE(in)) {
    case NPY_FLOAT:
    case NPY_DOUBLE:
    case NPY_BYTE:
    case NPY_INT:
        break;
    default:
        return false;
    }

    if (!PyArray_ISCONTIGUOUS(in))
        return false;

    return true;
}

bool NumpyArray2Array(PyObject* in, Adata** data)
{
  PyArrayObject* array = (PyArrayObject*)in;

  int M = PyArray_DIM(array,0);
  int N = PyArray_DIM(array,1);
  
  Adata * parray;
  void* pdata = array->data;
  switch (PyArray_TYPE(in)) {
  case NPY_FLOAT:
    parray = new Array<float, SINGLE>((float*)pdata, M, N);
    //std::cout<< "Converted to SINGLE" << std::endl;
    break;
  case NPY_DOUBLE:
    parray = new Array<double, DOUBLE>((double*)pdata, M, N);
    //std::cout<< "Converted to DOUBLE" << std::endl;
    break;
  case NPY_BYTE:
    parray = new Array<char, CHARACTER>((char*)pdata, M, N);
    //std::cout<< "Converted to CHARACTER" << std::endl;
    break;
  case NPY_INT:
    //std::cout<< "Converted to INTEGER" << std::endl;
    parray = new Array<int, INTEGER>((int*)pdata, M, N);
    break;
  default:
    return false;
  }
  *data = parray;
  
  return true;
}

bool Array2NumpyArray(Adata* data, PyObject** out)
{
  npy_intp dims[]= {data->rows(), data->cols()};
  int nd = 2;

  switch (data->type()) {
  case SINGLE:
    //std::cout<< "Converted to NPY_FLOAT" << std::endl;
    *out = PyArray_SimpleNewFromData(nd, dims, NPY_FLOAT, data->data());
    break;
  case DOUBLE:
    //std::cout<< "Converted to NPY_DOUBLE" << std::endl;
    *out = PyArray_SimpleNewFromData(nd, dims, NPY_DOUBLE, data->data());
    break;
  case CHARACTER:
    //std::cout<< "Converted to NPY_BYTE" << std::endl;
    *out = PyArray_SimpleNewFromData(nd, dims, NPY_BYTE, data->data());
    break;
  case INTEGER:
    //std::cout<< "Converted to NPY_INT" << std::endl;
    *out = PyArray_SimpleNewFromData(nd, dims, NPY_INT, data->data());
    break;
  default:
    return false;
  }
  
  return true;
}

template<class T>
void NumpyArray_copy(const T*__restrict data, const PyArrayObject* array, 
                     const int size)
{
  T*__restrict nparray = (T*)array->data;
  
  for(int i=0; i<size; i++)
    nparray[i] = data[i];
}

bool Array2NumpyArray_copy(Adata* data, PyObject** out)
{
  npy_intp dims[]= {data->rows(), data->cols()};
  int nd = 2;

  switch (data->type()) {
  case SINGLE:
    //std::cout<< "Converted to NPY_FLOAT" << std::endl;
    *out = PyArray_SimpleNew(nd, dims, NPY_FLOAT);
    NumpyArray_copy((float*)data->data(), (PyArrayObject*)*out, 
                    dims[0]*dims[1]);
    break;
  case DOUBLE:
    //std::cout<< "Converted to NPY_DOUBLE" << std::endl;
    *out = PyArray_SimpleNew(nd, dims, NPY_DOUBLE);
    NumpyArray_copy((double*)data->data(), (PyArrayObject*)*out, 
                    dims[0]*dims[1]);
    break;
  case CHARACTER:
    //std::cout<< "Converted to NPY_BYTE" << std::endl;
    *out = PyArray_SimpleNew(nd, dims, NPY_BYTE);
    NumpyArray_copy((char*)data->data(), (PyArrayObject*)*out, 
                    dims[0]*dims[1]);
    break;
  case INTEGER:
    //std::cout<< "Converted to NPY_INT" << std::endl;
    *out = PyArray_SimpleNew(nd, dims, NPY_INT);
    NumpyArray_copy((int*)data->data(), (PyArrayObject*)*out, 
                    dims[0]*dims[1]);
    break;
  default:
    return false;
  }

  return true;
}

void delete_array(Adata* data)
{
  delete data;
}
