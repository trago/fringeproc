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
   %apply (double* IN_ARRAY2, int DIM1, int DIM2) 
      {
        (double* I, int M, int N)
      };
   %apply (double** ARGOUTVIEW_FARRAY2, int* DIM1, int* DIM2)
      {
        (double** data, int* M, int* N)
      };
  %apply (signed char* IN_ARRAY2, int DIM1, int DIM2) 
     {
       (signed char* data, int M, int N)
     };

  Unwrap(double* I, int M, int N, double tau=0.09, 
         double smooth=9, int Nsize=15){
    cv::Mat_<double> dat(M, N);
    for(int i=0; i<M; i++)
      for(int j=0; j<N; j++)
	dat(i,j)=I[i + j*M];
    return new Unwrap(dat, tau, smooth, Nsize);
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

  void setMask(signed char* data, int M, int N){
    cv::Mat_<char> mask(M, N);
    for(int i=0; i<M; i++)
      for(int j=0; j<N; j++)
	mask(i,j)=data[i + j*M];    
    $self->setMask(mask);
  }

  %clear (double* I, int M, int N);
  %clear (double** data, int* M, int* N);
  %clear (signed char* data, int M, int N);
};

#endif
