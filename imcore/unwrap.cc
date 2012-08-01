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

#include <utils/utils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <utils/utils.h>
#include "unwrap.h"
#include "unwrap_gears.h"

inline
void sunwrap_neighborhood(const int ii, const int jj, const cv::Mat& wp,
                          const cv::Mat& mask,
                          cv::Mat& pp, cv::Mat& visited,
                          float tao, const int N)
{
  int low_i = (ii-N/2)>=0? (ii-N/2):0;
  int hig_i = (ii+N/2)<(wp.rows)? (ii+N/2):(wp.rows-1);
  int low_j = (jj-N/2)>=0? (jj-N/2):0;
  int hig_j = (jj+N/2)<(wp.cols)? (jj+N/2):(wp.cols-1);

  for(int i=low_i; i<=hig_i; i++){
    if(i%2==0)
      for(int j=low_j; j<=hig_j; j++){
        if(mask.at<char>(i,j)){
          pp.at<float>(i,j)=sunwrap_pixel(i*wp.cols+j, j, i,
                                          wp.ptr<float>(),
                                          mask.ptr<char>(),
                                          pp.ptr<float>(),
                                          visited.ptr<uchar>(),
                                          tao, wp.rows, wp.cols);
          visited.at<uchar>(i,j)=1;
        }
      }
    else
      for(int j=hig_j; j>=low_j; j--){
        if(mask.at<char>(i,j)){
          pp.at<float>(i,j)=sunwrap_pixel(i*wp.cols+j, j, i,
                                          wp.ptr<float>(),
                                          mask.ptr<char>(),
                                          pp.ptr<float>(),
                                          visited.ptr<uchar>(),
                                          tao, wp.rows, wp.cols);
          visited.at<uchar>(i,j)=1;
        }
      }
  }
}

inline
void dunwrap_neighborhood(const int ii, const int jj, const cv::Mat& wp,
                          const cv::Mat& mask,
                          cv::Mat& pp, cv::Mat& visited,
                          double tao, const int N)
{
  int low_i = (ii-N/2)>=0? (ii-N/2):0;
  int hig_i = (ii+N/2)<(wp.rows)? (ii+N/2):(wp.rows-1);
  int low_j = (jj-N/2)>=0? (jj-N/2):0;
  int hig_j = (jj+N/2)<(wp.cols)? (jj+N/2):(wp.cols-1);

  for(int i=low_i; i<=hig_i; i++){
    if(i%2==0)
      for(int j=low_j; j<=hig_j; j++){
        if(mask.at<char>(i,j)){
          pp.at<double>(i,j)=dunwrap_pixel(i*wp.cols+j, j, i,
                                           wp.ptr<double>(),
                                           mask.ptr<char>(),
                                           pp.ptr<double>(),
                                           visited.ptr<uchar>(),
                                           tao, wp.rows, wp.cols);
          visited.at<uchar>(i,j)=1;
        }
      }
    else
      for(int j=hig_j; j>=low_j; j--){
        if(mask.at<char>(i,j)){
          pp.at<double>(i,j)=dunwrap_pixel(i*wp.cols+j, j, i,
                                           wp.ptr<double>(),
                                           mask.ptr<char>(),
                                           pp.ptr<double>(),
                                           visited.ptr<uchar>(),
                                           tao, wp.rows, wp.cols);
          visited.at<uchar>(i,j)=1;
        }
      }
  }
}

/**
 * Phase unwrapping method
 */
template<typename T>
void unwrap2D_engine(cv::Mat wphase, cv::Mat mask, cv::Mat uphase,
                     double tao, double smooth_path, int n, cv::Point pixel)
{
  const int M=wphase.rows, N=wphase.cols;
  cv::Mat visited = cv::Mat::zeros(M, N, CV_8U);
  cv::Mat path, dx, dy;

  if(smooth_path>0){
    cv::GaussianBlur(wphase, path, cv::Size(0,0), smooth_path, smooth_path);
    gradient(path, dx, dy);
  }

  Scanner scan(dx, dy, pixel);
  scan.setMask(mask);
  int i,j, iter=0;
  if(wphase.type()==CV_32F)
    do{
      pixel = scan.getPosition();
      i=pixel.y;
      j=pixel.x;
      sunwrap_neighborhood(i, j, wphase, mask, uphase, visited, tao, n);
    }while(scan.next());
  else
    do{
      i=pixel.y;
      j=pixel.x;
      dunwrap_neighborhood(i, j, wphase, mask, uphase, visited, tao, n);
    }while(scan.next());

}

void unwrap2D(cv::Mat wphase, cv::Mat mask, cv::Mat uphase, double tao,
              double smooth_path, int N, cv::Point pixel) throw(cv::Exception)
{
  if(wphase.type()!=CV_32F && wphase.type()!=CV_64F){
    cv::Exception e(1000,
                    "Type not supported, must be single or double precision.",
                    "unwrap2D", std::string(__FILE__), __LINE__);
    throw(e);
  }
  if(uphase.rows!=wphase.rows && uphase.cols!=wphase.cols &&
     uphase.type()!=wphase.type())
    uphase.create(wphase.rows, wphase.cols, wphase.type());

  if(wphase.type()==CV_32F)
    unwrap2D_engine<float>(wphase, mask, uphase, tao, smooth_path, N, pixel);
  else
    unwrap2D_engine<double>(wphase, mask, uphase, tao, smooth_path, N, pixel);
}

Unwrap::Unwrap(cv::Mat_<double> wphase, double tau, double smooth, int N)
: _wphase(wphase)
{
  _tau = tau;
  _smooth = smooth;
  _N=N;
  _uphase = cv::Mat_<double>::zeros(_wphase.rows, _wphase.cols);
  _visited = cv::Mat_<char>::zeros(_wphase.rows, _wphase.cols);
  _mask = cv::Mat_<char>::ones(_wphase.rows, _wphase.cols);
  _dx = cv::Mat_<double>::zeros(_wphase.rows, _wphase.cols);
  _dy = cv::Mat_<double>::zeros(_wphase.rows, _wphase.cols);
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
    _scanner = new Scanner(_dx, _dy, _pixel);
    _scanner->setMask(_mask);
  }

  int iter=0;
  do{
    _pixel = _scanner->getPosition();
    int i= _pixel.y, j=_pixel.x;
    dunwrap_neighborhood(i, j, _wphase, _mask, _uphase, _visited, _tau, _N);
    //takeGradient(_pixel, _N);
    _visited(i,j)=1;
  }while(_scanner->next() && (++iter)<iters);

  return iter==iters;
}

void Unwrap::processPixel(cv::Point pixel)
{
  int i=pixel.y, j=pixel.x;
  dunwrap_neighborhood(i, j, _wphase, _mask, _uphase, _visited, _tau, _N);
}


void Unwrap::setPixel(cv::Point pixel)
{
  if(_scanner!=NULL){
    delete _scanner;
  }
  cv::Mat path;
  if(_smooth>0){
    cv::GaussianBlur(_wphase, path, cv::Size((int)_smooth,(int)_smooth),0);
    gradient(path, _dx, _dy);
  }
  _scanner = new Scanner(_dx, _dy, pixel);
  _scanner->setMask(_mask);
  _pixel=pixel;
}

cv::Mat Unwrap::getOutput()
{
  return _uphase;
}
cv::Mat Unwrap::getInput()
{
  return _wphase;
}

void Unwrap::setMask(cv::Mat mask)
{
  _mask = mask;
}

void Unwrap::filterPhase(double sigma)
{
  cv::Mat ss = sin<double>(_wphase);
  cv::Mat cc = cos<double>(_wphase);
  cv::GaussianBlur(ss, ss, cv::Size(0,0), sigma);
  cv::GaussianBlur(cc, cc, cv::Size(0,0), sigma);

  _wphase = atan2<double>(ss, cc);
}

cv::Mat Unwrap::genPath(double sigma)
{
  cv::Mat ss = sin<double>(_wphase);
  cv::Mat cc = cos<double>(_wphase);
  cv::GaussianBlur(ss, ss, cv::Size(0,0), sigma);
  cv::GaussianBlur(cc, cc, cv::Size(0,0), sigma);

  cv::Mat wphase = atan2<double>(ss, cc);
  cc = cos<double>(wphase);

  return cc;
}

void Unwrap::takeGradient(cv::Point pixel, const int N)
{
  const int N_2 = N/2;
  cv::Mat_<double>& p = _uphase;
  for(int i=pixel.y-N_2; i<=pixel.y+N_2; i++)
    for(int j=pixel.x-N_2; j<=pixel.x+N_2; j++){
      /*
      if(i>=1 && i<p.rows && j>=0 && j<p.cols)
        _dy(i,j)=(p(i,j)-p(i-1,j))*_mask(i,j);
      if(j>=1 && j<p.cols && i>=0 && i<p.rows)
        _dx(i,j)=(p(i,j)-p(i,j-1))*_mask(i,j);
      if(j==0 && j<p.cols && i>=0 && i<p.rows)
        _dx(i,j)=(p(i,j+1)-p(i,j))*_mask(i,j);
      if(i==0 && i<p.rows && j>=0 && j<p.cols)
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
