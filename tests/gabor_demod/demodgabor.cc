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

inline
void DemodGabor::filtraNeighborhood(double wx, double wy, int i, int j)
{
  if(i-1>=0)
   gabor_adaptiveFilterXY(m_I, m_fr, m_fi, wx, wy,
                          j, i-1);
  else if(i+1<m_I.rows)
   gabor_adaptiveFilterXY(m_I, m_fr, m_fi, wx, wy,
                          j, i+1);
  if(j-1>=0)
   gabor_adaptiveFilterXY(m_I, m_fr, m_fi, wx, wy,
                          j-1, i);
  else if(j+1<m_I.cols)
   gabor_adaptiveFilterXY(m_I, m_fr, m_fi, wx, wy,
                          j+1, i);
  gabor_adaptiveFilterXY(m_I, m_fr, m_fi, wx, wy,
                         j, i);
}

inline
void DemodGabor::demodPixelSeed(cv::Vec2d freqs, int i, int j)
{
  filtraNeighborhood (freqs[0], freqs[1], i,j);
  freqs = calc_freqXY(m_fr, m_fi, j, i);
  m_fx(i,j)=freqs[0];
  m_fy(i,j)=freqs[1];
  m_visited(i,j)=1;
}

inline
void DemodGabor::demodPixel(int i, int j)
{
  cv::Vec2d freqs;
  freqs= peak_freqXY(m_fx, m_fy, m_visited, j, i);
  filtraNeighborhood (freqs[0], freqs[1], i,j);
  freqs = calc_freqXY(m_fr, m_fi, j, i);

  m_fx(i,j)=freqs[0];
  m_fy(i,j)=freqs[1];
  m_visited(i,j)=1;
}

inline
void DemodGabor::demodNeighbor(int i, int j)
{
  cv::Mat_<char>& visit=(cv::Mat_<char>&)m_visited;
  
  demodPixel(i, j);
  if(j-1>=0)
    if(!visit(i,j-1))
      demodPixel(i, j-1);
  if(j+1<m_I.cols)
    if(!visit(i,j+1))
      demodPixel(i, j+1);
  if(i-1>=0)
    if(!visit(i-1,j))
      demodPixel(i-1, j);
  if(i+1<m_I.rows)
    if(!visit(i+1,j))
      demodPixel(i+1, j);
  if(j-1>=0 && i-1>=0)
    if(!visit(i-1,j-1))
      demodPixel(i-1, j-1);
  if(j+1<m_I.cols && i-1>=0)
    if(!visit(i-1,j+1))
      demodPixel(i-1, j+1);
  if(j+1<m_I.cols && i+1<m_I.rows)
    if(!visit(i+1,j+1))
      demodPixel(i+1, j+1);
  if(j-1>=0 && i+1<m_I.rows)
    if(!visit(i+1,j-1))
      demodPixel(i+1, j-1);
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
  
  do{
    pixel=scan.getPosition();
    i=pixel.y;
    j=pixel.x;
    if((i==p.y && j==p.x)){
      demodPixelSeed(freqs,i,j);
      freqs[0]=m_fx(i,j); freqs[1]=m_fy(i,j);
      demodPixelSeed(freqs,i,j);
      freqs[0]=m_fx(i,j); freqs[1]=m_fy(i,j);
      demodPixelSeed(freqs,i,j);
      freqs[0]=m_fx(i,j); freqs[1]=m_fy(i,j);
    }
    else
      demodNeighbor(i,j);
  }while(scan.next());
}
