/*
    Adaptive Gavor filter demodulation.
    Copyright (C) 2012  Julio C. Estrada julio@cio.mx

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "demodgabor.h"
#include "gabor_gears.h"
#include "scanner.h"
#include <opencv2/imgproc/imgproc.hpp>

DemodGabor::DemodGabor()
{
}

DemodGabor::DemodGabor(const cv::Mat I)
{
  CV_Assert(I.channels()==1);

  if(I.type() != CV_64F)
    I.convertTo(m_I, CV_64F);
  else
    m_I = I.clone();
  m_fr.create(m_I.rows, m_I.cols);
  m_fi.create(m_I.rows, m_I.cols);
  m_fx = cv::Mat_<double>::ones(m_I.rows, m_I.cols)*M_PI/2.0;
  m_fy = cv::Mat_<double>::ones(m_I.rows, m_I.cols)*M_PI/2.0;
  m_minf = 0.03;
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

void DemodGabor::setMinf(double minf)
{
  m_minf=minf;
}

void DemodGabor::removeDC()
{
  cv::Mat_<double> aux;
  cv::GaussianBlur(m_I, m_I, cv::Size(0,0), 1);
  cv::GaussianBlur(m_I, aux, cv::Size(0,0), 15);

  m_I-=aux;
}

void DemodGabor::run()
{
  cv::Vec2d freqs;
  cv::Point p(m_I.cols/2, m_I.rows/2); //Starting point
  int i=p.y, j=p.x, cont=0;
  freqs[0]=0.7; freqs[1]=0.7;

  m_fx(i,j)=freqs[0];
  m_fy(i,j)=freqs[1];
  m_visited = cv::Mat_<uchar>::zeros(m_I.rows, m_I.cols);

  Scanner scan(m_fx, m_fy, p);
  scan.setFreqMin(m_minf);
  cv::Point pixel;
  gabor::DemodNeighborhood demodN(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);
  gabor::DemodSeed demodSeed(m_I, m_fr, m_fi, m_fx, m_fy, m_visited);

  demodN.setIters(1).setKernelSize(7).
         setMaxFq(M_PI/4).setMinFq(0.09).setTau(0.25);
  demodSeed.setIters(9);
  
  do{
    pixel=scan.getPosition();
    i=pixel.y;
    j=pixel.x;
    if((i==p.y && j==p.x)){
      demodSeed(freqs,i,j);
    }
    else
      demodN(i,j);
  }while(scan.next());
}
