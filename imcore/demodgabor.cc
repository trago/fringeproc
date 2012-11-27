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
  m_startPixel(0)=0;
  m_startPixel(1)=0;
}

DemodGabor::DemodGabor(const Eigen::ArrayXXf& I)
{
  m_I = I;
  m_fr = Eigen::ArrayXXf::Zero(m_I.rows(), m_I.cols());
  m_fi = Eigen::ArrayXXf::Zero(m_I.rows(), m_I.cols());
  m_fx = Eigen::ArrayXXf::Constant(m_I.rows(), m_I.cols(), M_PI/2.0);
  m_fy = Eigen::ArrayXXf::Constant(m_I.rows(), m_I.cols(), M_PI/2.0);
  m_visited = Eigen::ArrayXXi::Zero(m_I.rows(), m_I.cols());
  m_scanMinf = 0.03;
  m_minfq = 0.09;
  m_maxfq = M_PI/2;
  m_iters = 1;
  m_seedIters = 9;
  m_tau = 0.25;
  m_kernelSize = 7;
  m_combSize=7;
  m_combFreqs=false;
  m_startPixel(0)=I.cols()/2;
  m_startPixel(1)=I.rows()/2;
}

Eigen::ArrayXXf DemodGabor::getFi()
{
  return m_fi;
}

Eigen::ArrayXXf DemodGabor::getFr()
{
  return m_fr;
}

Eigen::ArrayXXf DemodGabor::getInput()
{
  return m_I;
}

Eigen::ArrayXXf DemodGabor::getWx()
{
  return m_fx;
}

Eigen::ArrayXXf DemodGabor::getWy()
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
  Eigen::ArrayXXf aux;
  m_I = gaussian_filter(m_I, 1);
  aux = gaussian_filter(m_I, 15);

  m_I-=aux;
}

DemodGabor& DemodGabor::setStartPixel(const Eigen::Array2i& pixel)
{
  m_startPixel=pixel;
  return *this;
}

void DemodGabor::run()
{
  Eigen::Array2f freqs;
  int i=m_startPixel(1), j=m_startPixel(0);
  freqs(0)=0.7; freqs(1)=0.7;

  m_fx(i,j)=freqs(0);
  m_fy(i,j)=freqs(1);

  Scanner scan(&m_fx, &m_fy, m_startPixel);
  scan.setFreqMin(m_scanMinf);
  Eigen::Array2i pixel;
  gabor::DemodNeighborhood demodN(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);
  gabor::DemodSeed demodSeed(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);

  demodN.setIters(m_iters).setKernelSize(m_kernelSize).
    setCombFreqs(m_combFreqs).setCombSize(m_combSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);
  demodSeed.setIters(m_seedIters).setKernelSize(m_kernelSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);

  do{
    pixel=scan.getPosition();
    i=pixel(1);
    j=pixel(0);
    if((i==m_startPixel(1) && j==m_startPixel(0))){
      demodSeed(freqs,i,j);
      freqs(0)=m_fx(i,j); freqs(1)=m_fy(i,j);
      //scan.setFreqMin(sqrt(freqs[0]*freqs[0]+freqs[1]*freqs[1]));
    }
    else
      demodN(i,j);
  }while(scan.next());
}

bool DemodGabor::runInteractive(Scanner& scan)
{
  Eigen::Array2f freqs;
  freqs << 0.7,0.7;

  Eigen::Array2i pixel;
  gabor::DemodNeighborhood demodN(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);
  gabor::DemodSeed demodSeed(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);
  demodN.setIters(m_iters).setKernelSize(m_kernelSize).
    setCombFreqs(m_combFreqs).setCombSize(m_combSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);
  demodSeed.setIters(m_seedIters).setKernelSize(m_kernelSize).
    setMaxFq(m_maxfq).setMinFq(m_minfq).setTau(m_tau);

  pixel=scan.getPosition();
  const int i=pixel(1);
  const int j=pixel(0);
  if((i==m_startPixel(1) && j==m_startPixel(0))){
    demodSeed(freqs,i,j);
    freqs[0]=m_fx(i,j); freqs[1]=m_fy(i,j);
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
  m_fx = Eigen::ArrayXXf::Constant(m_I.rows(), m_I.cols(), M_PI/2.0);
  m_fy = Eigen::ArrayXXf::Constant(m_I.rows(), m_I.cols(), M_PI/2.0);
  m_visited = Eigen::ArrayXXi::Zero(m_I.rows(), m_I.cols());
  m_fr =  Eigen::ArrayXXf::Zero(m_I.rows(), m_I.cols());
  m_fi =  Eigen::ArrayXXf::Zero(m_I.rows(), m_I.cols());
}


Eigen::Array2i DemodGabor::getStartPixel()
{
  return m_startPixel;
}
