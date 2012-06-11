#ifndef CVMAPS_I
#define CVMAPS_I

%typemap(in) cv::Point{
  cv::Point point;
  PyObject *obj = $input;
  if(PySequence_Check(obj)){
    if(PySequence_Size(obj)==2){
      PyObject *pyX = PySequence_GetItem(obj, 0);
      PyObject *pyY = PySequence_GetItem(obj, 1);
      if(PyNumber_Check(pyX) && PyNumber_Check(pyY)){
	point.x=PyInt_AsLong(pyX);
	point.y=PyInt_AsLong(pyY);
      }
      else{
	PyErr_SetString(PyExc_TypeError, 
			"The sequence elements must be numbers [Integers]");
	return NULL;
      }
    }
    else{
      PyErr_SetString(PyExc_TypeError, 
		      "The sequence must represent a point od dimension 2");
      return NULL;
    }
  }
  else{
    PyErr_SetString(PyExc_TypeError, "Argument must be a sequence");
    return NULL;
  }
  $1=point;
 }

%typemap(out) cv::Point{
  $result = PyTuple_New(2);
  PyObject* aux = PyInt_FromLong(($1).x);
  PyTuple_SetItem($result, 0, aux);
  aux = PyInt_FromLong(($1).y);
  PyTuple_SetItem($result, 1, aux);
 }

#endif
