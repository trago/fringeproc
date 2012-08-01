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


#include "demodgabor.h"
#include "gabor_gears.h"
#include "scanner.h"
#include <opencv2/imgproc/imgproc.hpp>

DemodGabor::DemodGabor()
{
  m_scanMinf = 0.03;
  m_minfq = 0.09;
  m_maxfq = M_PI/2;
  m_iters = 1;
  m_seedIters = 9;
  m_tau = 0.25;
  m_kernelSize = 7;
  m_combSize=7;
  m_combFreqs=false;
  m_startPixel.x=0;
  m_startPixel.y=0;
}

DemodGabor::DemodGabor(const cv::Mat I)
{
  CV_Assert(I.channels()==1);

  if(I.type() != CV_64F)
    I.convertTo(m_I, CV_64F);
  else
    m_I = I.clone();
  m_fr = cv::Mat_<double>::zeros(m_I.rows, m_I.cols);
  m_fi = cv::Mat_<double>::zeros(m_I.rows, m_I.cols);
  m_fx = cv::Mat_<double>::ones(m_I.rows, m_I.cols)*M_PI/2.0;
  m_fy = cv::Mat_<double>::ones(m_I.rows, m_I.cols)*M_PI/2.0;
  m_visited = cv::Mat_<uchar>::zeros(m_I.rows, m_I.cols);
  m_scanMinf = 0.03;
  m_minfq = 0.09;
  m_maxfq = M_PI/2;
  m_iters = 1;
  m_seedIters = 9;
  m_tau = 0.25;
  m_kernelSize = 7;
  m_combSize=7;
  m_combFreqs=false;
  m_startPixel.x=I.cols/2;
  m_startPixel.y=I.rows/2;
}

cv::Mat DemodGabor::getFi()
{
  return m_fi;
}

cv::Mat DemodGabor::getFr()
{
  return m_fr;
}

cv::Mat DemodGabor::getInput()
{
  return m_I;
}

cv::Mat DemodGabor::getWx()
{
  return m_fx;
}

cv::Mat DemodGabor::getWy()
{
  return m_fy;
}

DemodGabor& DemodGabor::setScanMinf(double minf)
{
  m_scanMinf=minf;

  return *this;
}

void DemodGabor::removeDC()
{
  cv::Mat_<double> aux;
  cv::GaussianBlur(m_I, m_I, cv::Size(0,0), 1);
  cv::GaussianBlur(m_I, aux, cv::Size(0,0), 15);

  m_I-=aux;
}

DemodGabor& DemodGabor::setStartPixel(const cv::Point pixel)
{
  m_startPixel=pixel;
}

void DemodGabor::run()
{
  cv::Vec2d freqs;
  int i=m_startPixel.y, j=m_startPixel.x;
  freqs[0]=0.7; freqs[1]=0.7;

  m_fx(i,j)=freqs[0];
  m_fy(i,j)=freqs[1];

  Scanner scan(m_fx, m_fy, m_startPixel);
  scan.setFreqMin(m_scanMinf);
  cv::Point pixel;
  gabor::DemodNeighborhood demodN(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);
  gabor::DemodSeed demodSeed(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);

  demodN.setIters(m_iters).setKernelSize(m_kernelSize).
    setCombFreqs(m_combFreqs).setCombSize(m_combSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);
  demodSeed.setIters(m_seedIters).setKernelSize(m_kernelSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);

  do{
    pixel=scan.getPosition();
    i=pixel.y;
    j=pixel.x;
    if((i==m_startPixel.y && j==m_startPixel.x)){
      demodSeed(freqs,i,j);
      freqs[0]=m_fx.at<double>(i,j); freqs[1]=m_fy.at<double>(i,j);
      //scan.setFreqMin(sqrt(freqs[0]*freqs[0]+freqs[1]*freqs[1]));
    }
    else
      demodN(i,j);
  }while(scan.next());
}

bool DemodGabor::runInteractive(Scanner& scan)
{
  cv::Vec2d freqs(0.7,0.7);

  cv::Point pixel;
  gabor::DemodNeighborhood demodN(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);
  gabor::DemodSeed demodSeed(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);
  demodN.setIters(m_iters).setKernelSize(m_kernelSize).
    setCombFreqs(m_combFreqs).setCombSize(m_combSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);
  demodSeed.setIters(m_seedIters).setKernelSize(m_kernelSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);

  pixel=scan.getPosition();
  const int i=pixel.y;
  const int j=pixel.x;
  if((i==m_startPixel.y && j==m_startPixel.x)){
    demodSeed(freqs,i,j);
    freqs[0]=m_fx.at<double>(i,j); freqs[1]=m_fy.at<double>(i,j);
    //scan.setFreqMin(sqrt(freqs[0]*freqs[0]+freqs[1]*freqs[1]));
  }
  else
    demodN(i,j);

  return scan.next();
}

DemodGabor& DemodGabor::setIters(const int iters)
{
  m_iters=iters;
  return *this;
}
DemodGabor& DemodGabor::setSeedIters(const int iters)
{
  m_seedIters=iters;
  return *this;
}
DemodGabor& DemodGabor::setKernelSize(const double size)
{
  m_kernelSize=size;
  return *this;
}
DemodGabor& DemodGabor::setMaxfq(const double w)
{
  m_maxfq=w;
  return *this;
}
DemodGabor& DemodGabor::setMinfq(const double w)
{
  m_minfq=w;
  return *this;
}
DemodGabor& DemodGabor::setTau(const double tau)
{
  m_tau=tau;
  return *this;
}

DemodGabor& DemodGabor::setCombFreqs(const bool comb)
{
  m_combFreqs=comb;
  return *this;
}
DemodGabor& DemodGabor::setCombSize(const int size)
{
  m_combSize=size;
  return *this;
}

void DemodGabor::reset()
{
  m_fx = cv::Mat_<double>::ones(m_I.rows, m_I.cols)*M_PI/2.0;
  m_fy = cv::Mat_<double>::ones(m_I.rows, m_I.cols)*M_PI/2.0;
  m_visited = cv::Mat_<uchar>::zeros(m_I.rows, m_I.cols);
  m_fr =  cv::Mat_<double>::zeros(m_I.rows, m_I.cols);
  m_fi =  cv::Mat_<double>::zeros(m_I.rows, m_I.cols);
}


cv::Point DemodGabor::getStartPixel()
{
  return m_startPixel;
}
