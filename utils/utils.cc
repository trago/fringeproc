/**************************************************************************
Copyright (c) 2012, Julio C. Estrada
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

+ Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

+ Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************/

#include "utils.h"
#include <cmath>
#include <Eigen/Dense>

#define cimg_display 0
#include <CImg.h>
#include "utils_config.h"

void gradient(const Eigen::ArrayXXf& I, Eigen::ArrayXXf& dx, Eigen::ArrayXXf& dy)
{
  dx.resize(I.rows(), I.cols());
  dy.resize(I.rows(), I.cols());

  for(int i=0; i<I.rows()-1; i++)
    for(int j=0; j<I.cols()-1; j++){
      dx(i,j) = I(i,j+1)-I(i,j);
      dy(i,j) = I(i+1,j)-I(i,j);
    }
  for(int i=0; i<I.rows(); i++)
    dy(i,I.cols()-1)=dy(i,I.cols()-2);
  for(int i=0; i<I.cols(); i++)
    dx(I.rows()-1,i)=dx(I.rows()-2,i);
}

void parabola(Eigen::ArrayXXf& mat, float A)
{
  const int M=mat.rows(), N=mat.cols();
  const float cm=(float)(M/2.0), cn=(float)(N/2.0);


  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++)
      mat(i,j)= A*((i-cm)*(i-cm) + (j-cn)*(j-cn));
}

void cosine(const Eigen::ArrayXXf& angle, Eigen::ArrayXXf& cc)
{
  const int M=angle.rows(), N=angle.cols();
  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++)
      cc(i,j)=cos(angle(i,j));
}

void linspaced(Eigen::ArrayXXf& X, Eigen::ArrayXXf& Y, const float bx, float ex,
               const float by, float ey,
               const int M, const int N)
{
  X.resize(M,N);
  Y.resize(M,N);
  const float stepx = (ex-bx)/(float)(N-1);
  const float stepy = (ey-by)/(float)(M-1);
  float * _RESTRICT_ const ptx=(float*)X.data();
  float * _RESTRICT_ const pty=(float*)Y.data();
  size_t idx;

  float x=bx;
  float y=by;
  for(int i=0; i<M; i++){
    idx=N*i;
    x=bx;
    for(int j=0; j<N; j++){
      ptx[idx+j]=x;
      pty[idx+j]=y;
      x+=stepx;
    }
    y+=stepy;
  }
}

Eigen::ArrayXXf peaks(const int M, const int N)
{
  Eigen::ArrayXXf X, Y;
  Eigen::ArrayXXf p(M,N);
  Eigen::ArrayXXf tmp;
  linspaced(X, Y, -3.0, 3.0, -3.0, 3.0, M, N);

  p = 1 - X/2.0;
  tmp = X.pow(5);
  p+= tmp;
  tmp = Y.pow(3);
  p+= tmp;

  tmp = (-X*X-Y*Y).exp();
  p*=tmp;

  return p;
}

Eigen::ArrayXXf ramp(float wx, float wy, const int M, const int N)
{
  Eigen::ArrayXXf X, Y, res;
  linspaced(X, Y, 0.0f, (float)(M-1), 0.0f, (float)(N-1), M, N);
  res = wx*X + wy*Y;

  return res;
}

Eigen::ArrayXXf speckle_peaks(const int M, const int N, const float magn,
               const int speckle_size)
{
  using namespace cimg_library;
  Eigen::ArrayXXf Ic0;
  Eigen::ArrayXXf Ic1;
  Eigen::ArrayXXf p = peaks(M,N)*magn;
  Eigen::ArrayXXf noise0(M,N);
  Eigen::ArrayXXf noise1(M,N);
  CImg<float> n0(noise0.data(), noise0.cols(), noise0.rows(), 1, 1, true);
  CImg<float> n1(noise1.data(), noise1.cols(), noise1.rows(), 1, 1, true);
  const float pi=(float)(M_PI);

  float sigma = speckle_size/6.0f;
  n0.rand(-pi, pi);
  n1.rand(-pi, pi);
  n0.blur(sigma);
  n1.blur(sigma);
  noise0 = mapRange(noise0, -pi, pi);
  noise1 = mapRange(noise1, -pi, pi);

  Ic0 = wphase(noise0);
  Ic1 = wphase(noise1 + p + pi/2.0f);

  return wphase(Ic0-Ic1);
}

Eigen::ArrayXXf wphase(const Eigen::ArrayXXf& p)
{
  Eigen::ArrayXXf wp(p.rows(), p.cols());
  float *_RESTRICT_ const pt_wp = (float*)wp.data();
  float *_RESTRICT_ const pt_p = (float*)p.data();
  size_t idx;

  for(int i=0; i<p.rows(); i++){
    idx=i*p.cols();
    for(int j=0; j<p.cols(); j++)
      pt_wp[idx+j] = atan2(sin(pt_p[idx+j]), cos(pt_p[idx+j]));
  }

  return wp;
}

Eigen::ArrayXXf mapRange(const Eigen::ArrayXXf& mat, float a, float b)
{
  Eigen::ArrayXXf img;
  float min = mat.minCoeff();
  float max = mat.maxCoeff();

  float m = (b-a)/(max-min);

  img = m*(mat-min) + a;
  return img;
}
