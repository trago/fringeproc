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

#define cimg_display 0
#include <CImg.h>

#include <utils/utils.h>
#include <utils/utils.h>
#include "unwrap.h"
#include "unwrap_gears.h"
#include "gabor_gears.h"

inline
void sunwrap_neighborhood(const int ii, const int jj, const Eigen::ArrayXXf& wp,
                          const Eigen::ArrayXXi& mask,
                          Eigen::ArrayXXf& pp, Eigen::ArrayXXi& visited,
                          float tao, const int N)
{
  int low_i = (ii-N/2)>=0? (ii-N/2):0;
  int hig_i = (ii+N/2)<(wp.rows())? (ii+N/2):(wp.rows()-1);
  int low_j = (jj-N/2)>=0? (jj-N/2):0;
  int hig_j = (jj+N/2)<(wp.cols())? (jj+N/2):(wp.cols()-1);

  for(int i=low_i; i<=hig_i; i++){
    if(i%2==0)
      for(int j=low_j; j<=hig_j; j++){
        if(mask(i,j)){
          pp(i,j)=sunwrap_pixel(j*wp.rows()+i, j, i,
                                          wp.data(),
                                          mask.data(),
                                          pp.data(),
                                          visited.data(),
                                          tao, wp.rows(), wp.cols());
          visited(i,j)=1;
        }
      }
    else
      for(int j=hig_j; j>=low_j; j--){
        if(mask(i,j)){
          pp(i,j)=sunwrap_pixel(j*wp.rows()+i, j, i,
                                          wp.data(),
                                          mask.data(),
                                          pp.data(),
                                          visited.data(),
                                          tao, wp.rows(), wp.cols());
          visited(i,j)=1;
        }
      }
  }
}

inline
void dunwrap_neighborhood(const int ii, const int jj, const Eigen::ArrayXXf& wp,
                          const Eigen::ArrayXXi& mask,
                          Eigen::ArrayXXf& pp, Eigen::ArrayXXi& visited,
                          double tao, const int N)
{
  int low_i = (ii-N/2)>=0? (ii-N/2):0;
  int hig_i = (ii+N/2)<(wp.rows())? (ii+N/2):(wp.rows()-1);
  int low_j = (jj-N/2)>=0? (jj-N/2):0;
  int hig_j = (jj+N/2)<(wp.cols())? (jj+N/2):(wp.cols()-1);

  for(int i=low_i; i<=hig_i; i++){
    if(i%2==0)
      for(int j=low_j; j<=hig_j; j++){
        if(mask(i,j)){
          pp(i,j)=sunwrap_pixel(j*wp.rows()+i, j, i,
                                wp.data(),
                                mask.data(),
                                pp.data(),
                                visited.data(),
                                tao, wp.rows(), wp.cols());
          visited(i,j)=1;
        }
      }
    else
      for(int j=hig_j; j>=low_j; j--){
        if(mask(i,j)){
          pp(i,j)=sunwrap_pixel(j*wp.rows()+i, j, i,
                                           wp.data(),
                                           mask.data(),
                                           pp.data(),
                                           visited.data(),
                                           tao, wp.rows(), wp.cols());
          visited(i,j)=1;
        }
      }
  }
}

/**
 * Phase unwrapping method
 */
void unwrap2D_engine(const Eigen::ArrayXXf& wphase, const Eigen::ArrayXXi& mask,
                     Eigen::ArrayXXf& uphase,
                     double tao, double smooth_path, int n,
                     Eigen::Array2i pixel)
{
  const int M=wphase.rows(), N=wphase.cols();
  Eigen::ArrayXXi visited = Eigen::ArrayXXi::Zero(M, N);
  Eigen::ArrayXXf path, dx, dy;

  if(smooth_path>0){
    path = gaussian_filter(wphase, smooth_path);
    gradient(path, dx, dy);
  }

  Scanner scan(&dx, &dy, pixel);
  scan.setMask(mask);
  int i,j;
  do{
    pixel = scan.getPosition();
    i=pixel(0);
    j=pixel(1);
    sunwrap_neighborhood(i, j, wphase, mask, uphase, visited, tao, n);
  }while(scan.next());
}

void unwrap2D(const Eigen::ArrayXXf& wphase, const Eigen::ArrayXXi& mask,
              Eigen::ArrayXXf& uphase, double tao,
              double smooth_path, int N, Eigen::Array2i& pixel)
{
  uphase.resize(wphase.rows(), wphase.cols());

  unwrap2D_engine(wphase, mask, uphase, tao, smooth_path, N, pixel);
}

Unwrap::Unwrap(const Eigen::ArrayXXf& wphase, double tau, double smooth, int N)
: _wphase(wphase)
{
  _tau = tau;
  _smooth = smooth;
  _N=N;
  _uphase = Eigen::ArrayXXf::Zero(_wphase.rows(), _wphase.cols());
  _visited = Eigen::ArrayXXi::Zero(_wphase.rows(), _wphase.cols());
  _mask = Eigen::ArrayXXi::Constant(_wphase.rows(), _wphase.cols(), 1);
  _dx = Eigen::ArrayXXf::Zero(_wphase.rows(), _wphase.cols());
  _dy = Eigen::ArrayXXf::Zero(_wphase.rows(), _wphase.cols());
  _scanner = NULL;
}

Unwrap::~Unwrap()
{
  if (_scanner != NULL)
    delete _scanner;
}

void Unwrap::run()
{
  unwrap2D(_wphase, _mask, _uphase, _tau, _smooth, _N, _pixel);
}

bool Unwrap::runInteractive(int iters)
{
  if (_scanner==NULL) {
    _scanner = new Scanner(&_dx, &_dy, _pixel);
    _scanner->setMask(_mask);
  }

  int iter=0;
  do{
    _pixel = _scanner->getPosition();
    int i= _pixel(1), j=_pixel(0);
    dunwrap_neighborhood(i, j, _wphase, _mask, _uphase, _visited, _tau, _N);
    //takeGradient(_pixel, _N);
    _visited(i,j)=1;
  }while(_scanner->next() && (++iter)<iters);

  return !_scanner->empty();//iter==iters;
}

void Unwrap::processPixel(const Eigen::Array2i& pixel)
{
  int i=pixel(1), j=pixel(0);
  dunwrap_neighborhood(i, j, _wphase, _mask, _uphase, _visited, _tau, _N);
}


void Unwrap::setPixel(const Eigen::Array2i& pixel)
{
  using namespace cimg_library;
  if(_scanner!=NULL){
    delete _scanner;
  }
  Eigen::ArrayXXf path(_wphase.rows(), _wphase.rows());
  CImg<float> cimg_wphase(_wphase.data(), _wphase.cols(), _wphase.rows(),
                          1, 1, true);
  CImg<float> cimg_path(path.data(), _wphase.cols(), _wphase.rows(),
                          1, 1, true);
  if(_smooth>0){
    cimg_path = cimg_wphase.get_blur(_smooth);
    //cv::GaussianBlur(_wphase, path, cv::Size((int)_smooth,(int)_smooth),0);
    gradient(path, _dx, _dy);
  }
  _scanner = new Scanner(&_dx, &_dy, pixel);
  _scanner->setMask(_mask);
  _pixel=pixel;
}

Eigen::ArrayXXf Unwrap::getOutput()
{
  return _uphase;
}
Eigen::ArrayXXf Unwrap::getInput()
{
  return _wphase;
}

void Unwrap::setMask(const Eigen::ArrayXXi& mask)
{
  _mask = mask;
}

/*
void Unwrap::filterPhase(double sigma)
{
  using namespace cimg_library;
  Eigen::ArrayXXf ss = _wphase.sin();
  Eigen::ArrayXXf cc = _wphase.cos();
  CImg<float> cimg_ss(ss.data(), ss.cols(), ss.rows(), 1, 1, true);
  CImg<float> cimg_cc(cc.data(), cc.cols(), cc.rows(), 1, 1, true);

  cimg_ss.blur(sigma);
  cimg_cc.blur(sigma);



  for(int i=0; i<ss.rows(); i++)
    for(int j=0; j<ss.cols(); j++)
      _wphase(i,j) = atan2(ss, cc);
}
*/
Eigen::ArrayXXf Unwrap::genPath(double sigma)
{
  using namespace cimg_library;
  Eigen::ArrayXXf ss = _wphase.sin();
  Eigen::ArrayXXf cc = _wphase.cos();
  CImg<float> cimg_ss(ss.data(), ss.cols(), ss.rows(), 1, 1, true);
  CImg<float> cimg_cc(cc.data(), cc.cols(), cc.rows(), 1, 1, true);

  cimg_ss.blur(sigma);
  cimg_cc.blur(sigma);

  Eigen::ArrayXXf wphase(ss.rows(), ss.cols());
  for(int i=0; i<ss.rows(); i++)
    for(int j=0; j<ss.cols(); j++)
      wphase(i,j) = atan2(ss(i,j),cc(i,j));
  cc = wphase.cos();

  return cc;
}

void Unwrap::takeGradient(const Eigen::Array2i& pixel, const int N)
{
  const int N_2 = N/2;
  Eigen::ArrayXXf& p = _uphase;
  for(int i=pixel(1)-N_2; i<=pixel(1)+N_2; i++)
    for(int j=pixel(0)-N_2; j<=pixel(0)+N_2; j++){
      /*
      if(i>=1 && i<p.rows() && j>=0 && j<p.cols())
        _dy(i,j)=(p(i,j)-p(i-1,j))*_mask(i,j);
      if(j>=1 && j<p.cols() && i>=0 && i<p.rows())
        _dx(i,j)=(p(i,j)-p(i,j-1))*_mask(i,j);
      if(j==0 && j<p.cols() && i>=0 && i<p.rows())
        _dx(i,j)=(p(i,j+1)-p(i,j))*_mask(i,j);
      if(i==0 && i<p.rows() && j>=0 && j<p.cols())
        _dy(i,j)=(p(i+1,j)-p(i,j))*_mask(i,j);
      */
      _dy(i,j)=p(i,j);
      _dx(i,j)=p(i,j);
    }

}

void Unwrap::setTao(double tao)
{
  _tau = tao;
}
