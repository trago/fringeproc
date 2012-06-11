/* -*- C -*-  (not really, but good for syntax highlighting) */
#ifndef DEMODGABOR
#define DEMODGABOR

%{
  #include "demodgabor.h"
%}

%include "scanner.i"
%include "numpy.i"

%ignore DemodGabor(const cv::Mat I);
%ignore getFr();
%ignore getFi();
%ignore getWx();
%ignore getWy();
%ignore getInput();

%include "demodgabor.h"

%extend DemodGabor{
 public:
  %apply (double* IN_FARRAY2, int DIM1, int DIM2) 
     {
       (double* I, int M, int N)
     };
  %apply (double** ARGOUTVIEW_FARRAY2, int* DIM1, int* DIM2)
     {
       (double** data, int* M, int* N)
     };

  DemodGabor(double* I, int M, int N){
    cv::Mat data(M, N, CV_64F, I);
    return new DemodGabor(data);
  }

  void getFr(double** data, int* M, int *N){
    cv::Mat mat = $self->getFr();
    *data=mat.ptr<double>();
    *M=mat.rows;
    *N=mat.cols;
  } 

  void getFi(double** data, int* M, int *N){
    cv::Mat mat = $self->getFr();
    *data=mat.ptr<double>();
    *M=mat.rows;
    *N=mat.cols;
  } 

  void getWx(double** data, int* M, int *N){
    cv::Mat mat = $self->getFr();
    *data=mat.ptr<double>();
    *M=mat.rows;
    *N=mat.cols;
  } 

  void getWy(double** data, int* M, int *N){
    cv::Mat mat = $self->getFr();
    *data=mat.ptr<double>();
    *M=mat.rows;
    *N=mat.cols;
  } 

  void getInput(double** data, int* M, int *N){
    cv::Mat mat = $self->getFr();
    *data=mat.ptr<double>();
    *M=mat.rows;
    *N=mat.cols;
  } 
 };

#endif
