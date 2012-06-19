/* -*- C -*-  (not really, but good for syntax highlighting) */
#ifndef UNWRAP
#define UNWRAP

%{
#include "unwrap.h"
%}

%include "numpy.i"
%include "cvmaps.i"
%include "unwrap.h"

%extend Unwrap{
 public:
   %apply (double* IN_FARRAY2, int DIM1, int DIM2) 
      {
        (double* I, int M, int N)
      };
   %apply (double** ARGOUTVIEW_FARRAY2, int* DIM1, int* DIM2)
      {
        (double** data, int* M, int* N)
      };

  Scanner(double* I, int M, int N, double tau=0.09, 
          double smooth=9, int N=15){
    cv::Mat dat(M1, N1, CV_64F, dat1);
    return new UnwrapScanner(dat, tau, smooth, N);
  }

  void getOutput(double** data, int* M, int *N){
    cv::Mat mat = $self->getOutput();
    *data=mat.ptr<double>();
    *M=mat.rows;
    *N=mat.cols;
  }

  void getInput(double** data, int* M, int *N){
    cv::Mat mat = $self->getInput();
    *data = mat.ptr<double>();
    *M=mat.rows;
    *N=mat.cols;
  }


};

#endif