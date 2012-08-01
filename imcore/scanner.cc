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

#include "scanner.h"
#include <algorithm>
#include <iostream>

Scanner::Scanner()
{
  m_freqmin=0.6;
  m_pixel.x=0;
  m_pixel.y=0;
  m_updateMinFreq=true;
}

Scanner::Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v)
{
  CV_Assert(mat_u.type()==CV_64F && mat_v.type()==CV_64F);

  m_matu=mat_u;
  m_matv=mat_v;
  m_visited = cv::Mat_<bool>::zeros(mat_u.rows, mat_u.cols);
  m_mask = cv::Mat_<bool>::ones(mat_u.rows, mat_u.cols);
  m_pixel.x=0;
  m_pixel.y=0;
  insertPixelToPath(m_pixel);
  m_freqmin=0.6;
  m_updateMinFreq=true;
}
Scanner::Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v, cv::Point pixel)
{
  CV_Assert(mat_u.type()==CV_64F && mat_v.type()==CV_64F);

  m_matu=mat_u;
  m_matv=mat_v;
  m_visited = cv::Mat_<bool>::zeros(mat_u.rows, mat_u.cols);
  m_mask = cv::Mat_<bool>::ones(mat_u.rows, mat_u.cols);
  m_pixel=pixel;
  insertPixelToPath(m_pixel);
  m_freqmin=0.6;
  m_updateMinFreq=true;
}

void Scanner::setFreqMin(double freq)
{
  m_freqmin = freq;
}

void Scanner::setMask(cv::Mat mask)
{
  m_mask = mask;
}

bool Scanner::next()
{
  if(!next(m_freqmin*m_freqmin)){
    m_pixel=findPixel();
    if(m_pixel.x>=0 && m_pixel.y>=0 && m_updateMinFreq){
      //std::cout<<"Frequencia actual: "<<m_freqmin;
      m_freqmin=sqrt(m_matu(m_pixel.y,m_pixel.x)*m_matu(m_pixel.y,m_pixel.x)
        + m_matv(m_pixel.y,m_pixel.x)*m_matv(m_pixel.y,m_pixel.x));
      //m_freqmin-=0.01;
      //std::cout<<", Frecuencia ajustada: "<<m_freqmin<<std::endl;
      //std::cout<<"Nuevo punto inicial: (" << m_pixel.x << ", " <<m_pixel.y
      //         << ")" << std::endl;
      insertPixelToPath(m_pixel);
      return true;//next(m_freqmin*m_freqmin);
    }
    return false;
  }
  return true;

  //return next(m_freqmin*m_freqmin);
}

bool Scanner::checkNeighbor(cv::Point pixel)
{
  int y=pixel.y, x=pixel.x;

  if(!m_visited(y,x) && m_mask(y,x)){
    if(x-1>=0)
      if(m_visited(y,x-1) && m_mask(y,x-1))
        return true;
    if(x+1<m_matu.cols)
      if(m_visited(y,x+1) && m_mask(y,x+1))
        return true;
    if(y-1>=0)
      if(m_visited(y-1,x) && m_mask(y-1,x))
        return true;
    if(y+1<m_matu.rows)
      if(m_visited(y+1,x) && m_mask(y+1,x))
        return true;
    if(x-1>=0 && y-1>=0)
      if(m_visited(y-1,x-1) && m_mask(y-1,x-1))
        return true;
    if(x+1<m_matu.cols && y-1>=0)
      if(m_visited(y-1,x+1) && m_mask(y-1,x+1))
        return true;
    if(x+1<m_matu.cols && y+1<m_matu.rows)
      if(m_visited(y+1,x+1) && m_mask(y+1,x+1))
        return true;
    if(x-1>=0 && y+1<m_matu.rows)
      if(m_visited(y+1,x-1) && m_mask(y+1,x-1))
        return true;
  }
  return false;
}

cv::Point Scanner::findPixel()
{
  cv::Point pixel;
  int &x=pixel.x, &y=pixel.y;
  m_pixel.x=(m_pixel.x>=0 && m_pixel.x<m_visited.cols)? m_pixel.x:0;
  m_pixel.y=(m_pixel.y>=0 && m_pixel.y<m_visited.rows)? m_pixel.y:0;

  for(y=m_pixel.y; y<m_visited.rows; y++)
    for(x=m_pixel.x; x<m_visited.cols; x++){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  for(y=m_pixel.y; y>=0; y--)
    for(x=m_pixel.x; x<m_visited.cols; x++){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  for(y=m_pixel.y; y>=0; y--)
    for(x=m_pixel.x; x>=0; x--){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  for(y=m_pixel.y; y<m_visited.rows; y++)
    for(x=m_pixel.x; x>=0; x--){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  return cv::Point(-1,-1);
}

inline
bool Scanner::next(double fpow)
{
  cv::Vec<double, 8> magn;
  std::vector<cv::Point> pixel(8,cv::Point(0,0));
  while(!m_path.empty()){
    m_pixel=m_path.back();
    for(int i=0; i<8; i++) magn[i]=-1;
    int x=m_pixel.x, y=m_pixel.y;
    if(x-1>=0)
      if(!m_visited(y,x-1) && m_mask(y,x-1)){
        pixel[0]=cv::Point(x-1,y);
        magn[0]=m_matu(y,x-1)*m_matu(y,x-1) + m_matv(y,x-1)*m_matv(y,x-1);
      }
    if(x+1<m_matu.cols)
      if(!m_visited(y,x+1) && m_mask(y,x+1)){
        pixel[1]=cv::Point(x+1,y);
        magn[1]=m_matu(y,x+1)*m_matu(y,x+1) + m_matv(y,x+1)*m_matv(y,x+1);
      }
    if(y-1>=0)
      if(!m_visited(y-1,x) && m_mask(y-1,x)){
        pixel[2]=cv::Point(x,y-1);
        magn[2]=m_matu(y-1,x)*m_matu(y-1,x) + m_matv(y-1,x)*m_matv(y-1,x);
      }
    if(y+1<m_matu.rows)
      if(!m_visited(y+1,x) && m_mask(y+1,x)){
        pixel[3]=cv::Point(x,y+1);
        magn[3]=m_matu(y+1,x)*m_matu(y+1,x) + m_matv(y+1,x)*m_matv(y+1,x);
      }
    if(x-1>=0 && y-1>=0)
      if(!m_visited(y-1,x-1) && m_mask(y-1,x-1)){
        pixel[4]=cv::Point(x-1,y-1);
        magn[4]=m_matu(y-1,x-1)*m_matu(y-1,x-1) +
            m_matv(y-1,x-1)*m_matv(y-1,x-1);
      }
    if(x+1<m_matu.cols && y-1>=0)
      if(!m_visited(y-1,x+1) && m_mask(y-1,x+1)){
        pixel[5]=cv::Point(x+1,y-1);
        magn[5]=m_matu(y-1,x+1)*m_matu(y-1,x+1) +
            m_matv(y-1,x+1)*m_matv(y-1,x+1);
      }
    if(x+1<m_matu.cols && y+1<m_matu.rows)
      if(!m_visited(y+1,x+1) && m_mask(y+1,x+1)){
        pixel[6]=cv::Point(x+1,y+1);
        magn[6]=m_matu(y+1,x+1)*m_matu(y+1,x+1) +
            m_matv(y+1,x+1)*m_matv(y+1,x+1);
      }
    if(x-1>=0 && y+1<m_matu.rows)
      if(!m_visited(y+1,x-1) && m_mask(y+1,x-1)){
        pixel[7]=cv::Point(x-1,y+1);
        magn[7]=m_matu(y+1,x-1)*m_matu(y+1,x-1) +
            m_matv(y+1,x-1)*m_matv(y+1,x-1);
      }

    int idx=0;
    double mayor=magn[0];
    bool found=mayor>=fpow? true:false;
    for(int i=1; i<8; i++)
      if(mayor<=magn[i] && magn[i]>=fpow){
        mayor=magn[i];
        idx=i;
        found=true;
      }
    if(found){
      m_pixel=pixel[idx];
      insertPixelToPath(m_pixel);
      //CompPoints compara(m_matu, m_matv);
      //m_path.sort(compara);
      //m_pixel=m_path.back();
      return true;
    }
    else
      m_path.pop_back();
  }
  return false;
}

void Scanner::setInitPosition(cv::Point pixel)
{
  m_pixel=pixel;
  insertPixelToPath(m_pixel);
}

cv::Point2i Scanner::getPosition()
{
  return m_pixel;
}

inline
void Scanner::insertPixelToPath(const cv::Point& pixel)
{
  if(m_mask(pixel.y, pixel.x)){
    m_visited(pixel.y, pixel.x)=true;
    m_path.push_back(pixel);
  }
}

void Scanner::updateFreqMin(bool update)
{
  m_updateMinFreq=update;
}
