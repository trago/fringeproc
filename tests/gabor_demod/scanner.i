/* -*- C -*-  (not really, but good for syntax highlighting) */
#ifndef SCANNER
#define SCANNER

%{
#include "scanner.h"
%}

%include "numpy.i"
%include "cvmaps.i"
%include "scanner.h"

%extend Scanner{
 public:
  %apply (int DIM1, int DIM2, double* IN_ARRAY2) 
     {(int M1, int N1, double* dat1), (int M2, int N2, double* dat2)};
  %apply cv::Point {cv::Point pixel};

  Scanner(int M1, int N1, double* dat1, 
	  int M2, int N2, double* dat2){
    cv::Mat u(M1, N1, CV_64F, dat1), v(M2, N2, CV_64F, dat2);
    return new Scanner(u, v);
  }
  Scanner(int M1, int N1, double* dat1, 
	  int M2, int N2, double* dat2, cv::Point pixel){
    cv::Mat u(M1, N1, CV_64F, dat1), v(M2, N2, CV_64F, dat2);
    return new Scanner(u, v, pixel);
  }
 };

#endif
