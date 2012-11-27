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
  :m_matu(NULL), m_matv(NULL)
{
  m_freqmin=0.6;
  m_pixel(0)=0;
  m_pixel(1)=0;
  m_updateMinFreq=true;
}

Scanner::Scanner(const Eigen::ArrayXXf* mat_u, const Eigen::ArrayXXf* mat_v)
  :m_matu(mat_u), m_matv(mat_v)
{
  m_visited = Eigen::ArrayXXf::Zero(mat_u->rows(), mat_u->cols());
  m_mask = Eigen::ArrayXXf::Zero(mat_u->rows(), mat_u->cols());
  m_pixel(0)=0;
  m_pixel(1)=0;
  insertPixelToPath(m_pixel);
  m_freqmin=0.6;
  m_updateMinFreq=true;
}
Scanner::Scanner(const Eigen::ArrayXXf* mat_u, const Eigen::ArrayXXf* mat_v,
                 Eigen::Array2i pixel)
  :m_matu(mat_u), m_matv(mat_v)
{
  m_visited = Eigen::ArrayXXf::Zero(mat_u->rows(), mat_u->cols());
  m_mask = Eigen::ArrayXXf::Zero(mat_u->rows(), mat_u->cols());
  m_pixel=pixel;
  insertPixelToPath(m_pixel);
  m_freqmin=0.6;
  m_updateMinFreq=true;
}

void Scanner::setFreqMin(double freq)
{
  m_freqmin = freq;
}

void Scanner::setMask(Eigen::ArrayXXf mask)
{
  m_mask = mask;
}

bool Scanner::next()
{
  const Eigen::ArrayXXf& matu = *m_matu;
  const Eigen::ArrayXXf& matv = *m_matv;
  if(!next(m_freqmin*m_freqmin)){
    m_pixel=findPixel();
    if(m_pixel(0)>=0 && m_pixel(1)>=0 && m_updateMinFreq){
      //std::cout<<"Frequencia actual: "<<m_freqmin;
      m_freqmin=sqrt(matu(m_pixel(1),m_pixel(0))*matu(m_pixel(1),m_pixel(0))
        + matv(m_pixel(1),m_pixel(0))*matv(m_pixel(1),m_pixel(0)));
      //m_freqmin-=0.01;
      //std::cout<<", Frecuencia ajustada: "<<m_freqmin<<std::endl;
      //std::cout<<"Nuevo punto inicial: (" << m_pixel(0) << ", " <<m_pixel(1)
      //         << ")" << std::endl;
      insertPixelToPath(m_pixel);
      return true;//next(m_freqmin*m_freqmin);
    }
    return false;
  }
  return true;

  //return next(m_freqmin*m_freqmin);
}

bool Scanner::checkNeighbor(Eigen::Array2i pixel)
{
  int y=pixel(1), x=pixel(0);

  if(!m_visited(y,x) && m_mask(y,x)){
    if(x-1>=0)
      if(m_visited(y,x-1) && m_mask(y,x-1))
        return true;
    if(x+1<m_matu->cols())
      if(m_visited(y,x+1) && m_mask(y,x+1))
        return true;
    if(y-1>=0)
      if(m_visited(y-1,x) && m_mask(y-1,x))
        return true;
    if(y+1<m_matu->rows())
      if(m_visited(y+1,x) && m_mask(y+1,x))
        return true;
    if(x-1>=0 && y-1>=0)
      if(m_visited(y-1,x-1) && m_mask(y-1,x-1))
        return true;
    if(x+1<m_matu->cols() && y-1>=0)
      if(m_visited(y-1,x+1) && m_mask(y-1,x+1))
        return true;
    if(x+1<m_matu->cols() && y+1<m_matu->rows())
      if(m_visited(y+1,x+1) && m_mask(y+1,x+1))
        return true;
    if(x-1>=0 && y+1<m_matu->rows())
      if(m_visited(y+1,x-1) && m_mask(y+1,x-1))
        return true;
  }
  return false;
}

Eigen::Array2i Scanner::findPixel()
{
  Eigen::Array2i pixel;
  int &x=pixel(0), &y=pixel(1);
  m_pixel(0)=(m_pixel(0)>=0 && m_pixel(0)<m_visited.cols())? m_pixel(0):0;
  m_pixel(1)=(m_pixel(1)>=0 && m_pixel(1)<m_visited.rows())? m_pixel(1):0;

  for(y=m_pixel(1); y<m_visited.rows(); y++)
    for(x=m_pixel(0); x<m_visited.cols(); x++){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  for(y=m_pixel(1); y>=0; y--)
    for(x=m_pixel(0); x<m_visited.cols(); x++){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  for(y=m_pixel(1); y>=0; y--)
    for(x=m_pixel(0); x>=0; x--){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  for(y=m_pixel(1); y<m_visited.rows(); y++)
    for(x=m_pixel(0); x>=0; x--){
      if(checkNeighbor(pixel)){
        return pixel;
      }
    }
  return Eigen::Array2i(-1,-1);
}

inline
bool Scanner::next(double fpow)
{
  const Eigen::ArrayXXf& matu = *m_matu;
  const Eigen::ArrayXXf& matv = *m_matv;
  Eigen::ArrayXf magn(8);
  std::vector<Eigen::Array2i> pixel(8,Eigen::Array2i(0,0));
  while(!m_path.empty()){
    m_pixel=m_path.back();
    for(int i=0; i<8; i++) magn[i]=-1;
    int x=m_pixel(0), y=m_pixel(1);
    if(x-1>=0)
      if(!m_visited(y,x-1) && m_mask(y,x-1)){
        pixel[0]=(Eigen::Array2i() << x-1, y).finished();
        magn[0]=matu(y,x-1)*matu(y,x-1) + matv(y,x-1)*matv(y,x-1);
      }
    if(x+1<matu.cols())
      if(!m_visited(y,x+1) && m_mask(y,x+1)){
        pixel[1]=(Eigen::Array2i() << x+1, y).finished();
        magn[1]=matu(y,x+1)*matu(y,x+1) + matv(y,x+1)*matv(y,x+1);
      }
    if(y-1>=0)
      if(!m_visited(y-1,x) && m_mask(y-1,x)){
        pixel[2]=(Eigen::Array2i() << x, y-1).finished();
        magn[2]=matu(y-1,x)*matu(y-1,x) + matv(y-1,x)*matv(y-1,x);
      }
    if(y+1<matu.rows())
      if(!m_visited(y+1,x) && m_mask(y+1,x)){
        pixel[3]=(Eigen::Array2i() << x, y+1).finished();
        magn[3]=matu(y+1,x)*matu(y+1,x) + matv(y+1,x)*matv(y+1,x);
      }
    if(x-1>=0 && y-1>=0)
      if(!m_visited(y-1,x-1) && m_mask(y-1,x-1)){
        pixel[4]=(Eigen::Array2i() << x-1, y-1).finished();
        magn[4]=matu(y-1,x-1)*matu(y-1,x-1) +
            matv(y-1,x-1)*matv(y-1,x-1);
      }
    if(x+1<matu.cols() && y-1>=0)
      if(!m_visited(y-1,x+1) && m_mask(y-1,x+1)){
        pixel[5]=(Eigen::Array2i() << x+1, y-1).finished();
        magn[5]=matu(y-1,x+1)*matu(y-1,x+1) +
            matv(y-1,x+1)*matv(y-1,x+1);
      }
    if(x+1<matu.cols() && y+1<matu.rows())
      if(!m_visited(y+1,x+1) && m_mask(y+1,x+1)){
        pixel[6]=(Eigen::Array2i() << x+1, y+1).finished();
        magn[6]=matu(y+1,x+1)*matu(y+1,x+1) +
            matv(y+1,x+1)*matv(y+1,x+1);
      }
    if(x-1>=0 && y+1<matu.rows())
      if(!m_visited(y+1,x-1) && m_mask(y+1,x-1)){
        pixel[7]=(Eigen::Array2i() << x-1, y+1).finished();
        magn[7]=matu(y+1,x-1)*matu(y+1,x-1) +
            matv(y+1,x-1)*matv(y+1,x-1);
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

void Scanner::setInitPosition(Eigen::Array2i pixel)
{
  m_pixel=pixel;
  insertPixelToPath(m_pixel);
}

Eigen::Array2i Scanner::getPosition()
{
  return m_pixel;
}

inline
void Scanner::insertPixelToPath(const Eigen::Array2i& pixel)
{
  if(m_mask(pixel(1), pixel(0))){
    m_visited(pixel(1), pixel(0))=true;
    m_path.push_back(pixel);
  }
}

void Scanner::updateFreqMin(bool update)
{
  m_updateMinFreq=update;
}
