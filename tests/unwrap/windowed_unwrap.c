#include <mex.h>
#include <stdlib.h>
#include <stdio.h>
#include "c_unwrap.h"

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
  int nr, nc, N, x, y;
  float tau, sigma;
  double *wphase, *uphase;
  float *swphase, *suphase;
  int i;

  if(nrhs!=6) {
      mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:nrhs",
                        "Four inputs required.");
  }
  if(nlhs!=1) {
      mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:nlhs",
                        "One output required.");
  }
  if( !mxIsDouble(prhs[0]) ||
      mxGetNumberOfElements(prhs[0])==1 ) {
    mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:notMatrix",
                      "Input wphase must be a matrix.");
  }
  nr=mxGetM(prhs[0]);
  nc=mxGetN(prhs[0]);

  if( !mxIsDouble(prhs[1]) ||
      mxGetNumberOfElements(prhs[1])!=1 ) {
    mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:notMatrix",
                      "Input tau must be a scalar.");
  }
  tau=mxGetScalar(prhs[1]);

  if( !mxIsDouble(prhs[2]) ||
      mxGetNumberOfElements(prhs[2])!=1 ) {
    mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:notMatrix",
                      "Input sigma must be a scalar.");
  }
  sigma=mxGetScalar(prhs[2]);

  if( !mxIsDouble(prhs[3]) ||
      mxGetNumberOfElements(prhs[3])!=1 ) {
    mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:notMatrix",
                      "Input N must be a scalar.");
  }
  N=(int)mxGetScalar(prhs[3]);

  if( !mxIsDouble(prhs[4]) ||
      mxGetNumberOfElements(prhs[4])!=1 ) {
    mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:notMatrix",
                      "Input x must be a scalar.");
  }
  x=(int)mxGetScalar(prhs[4]);

  if( !mxIsDouble(prhs[5]) ||
      mxGetNumberOfElements(prhs[5])!=1 ) {
    mexErrMsgIdAndTxt("fringeproc:windowed_unwrap:notMatrix",
                      "Input y must be a scalar.");
  }
  y=(int)mxGetScalar(prhs[5]);

  plhs[0] = mxCreateDoubleMatrix(nr,nc,mxREAL);
  uphase = mxGetPr(plhs[0]);
  wphase = mxGetPr(prhs[0]);

  suphase = (float*)malloc(sizeof(float)*nr*nc);
  swphase = (float*)malloc(sizeof(float)*nr*nc);

  for(i=0; i<nr*nc; i++)
    swphase[i]=wphase[i];

  unwrap_run(swphase, suphase, nr, nc, tau, sigma, x, y, N);

  for(i=0; i<nr*nc; i++)
    uphase[i]=suphase[i];

  free(suphase);
  free(swphase);

}
