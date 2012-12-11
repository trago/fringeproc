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

#include <cmath>
#include <iostream>
#include "gabor_gears.h"
#include "imcore_config.h"

/**
 */
inline
double dconvolutionAtXY(const double *_RESTRICT_ data,
               const double *_RESTRICT_ kernelx,
               const double *_RESTRICT_ kernely,
               const int x, const int y,
               const int M, const int N,
               const int kM, const int kN)
{
  const int LIx = (x-kN)>=0? -kN:-x;
  const int LSx = (x+kN)<N?  kN:N-x-1;
  const int LIy = (y-kM)>=0? -kM:-y;
  const int LSy = (y+kM)<M? kM:M-y-1;

  double sum=0;
  //double weight=0;
  double f =0;
  for(int i=LIy; i<=LSy; i++){
    f=0;
    for(int j=LIx; j<=LSx; j++)
      f+= data[(y+i)*N + j+x]*kernelx[kN+j];
    sum+=f*kernely[kM+i];
    //weight+=kernely[kM+i];
  }

  return sum;//weight!=0? sum/weight:0;
}

float convolutionAtXY(const Eigen::ArrayXXf& data,
                      const Eigen::ArrayXXf& kernelx,
                      const Eigen::ArrayXXf& kernely, const int x, const int y)
{
  return sconvolutionAtXY(data.data(), kernelx.data(),
                          kernely.data(), x, y,
                          data.rows(), data.cols(), kernely.cols()/2,
                          kernelx.cols()/2);
}

inline
void dconvolution(const double *_RESTRICT_ data,
         const double *_RESTRICT_ kernelx,
         const double *_RESTRICT_ kernely,
         double *_RESTRICT_ out,
         const int M, const int N,
         const int KM, const int KN)
{
  const int kM=KM/2;
  const int kN=KN/2;
  for(int y=0; y<M; y++)
    for(int x=0; x<N; x++){
      int LIx = (x-kN)>=0? -kN:-x;
      int LSx = (x+kN)<N?  kN:N-x-1;
      int LIy = (y-kM)>=0? -kM:-y;
      int LSy = (y+kM)<M? kM:M-y-1;

      double sum=0;
      //double weight=0;
      double f =0;
      for(int i=LIy; i<=LSy; i++){
        f=0;
        for(int j=LIx; j<=LSx; j++)
          f+= data[(y+i)*N + j+x]*kernelx[kN+j];
        sum+=f*kernely[kM+i];
        //weight+=kernely[kM+i];
      }
      out[y*N + x]=sum; //sum/weight;
    }
}

//inline
float sconvolutionAtXY(const float *_RESTRICT_ data,
               const float *_RESTRICT_ kernelx,
               const float *_RESTRICT_ kernely,
               const int x, const int y,
               const int M, const int N,
               const int kM, const int kN)
{
  const int LIx = (x-kN)>=0? -kN:-x;
  const int LSx = (x+kN)<N?  kN:N-x-1;
  const int LIy = (y-kM)>=0? -kM:-y;
  const int LSy = (y+kM)<M? kM:M-y-1;

  double sum=0;
  //double weight=0;
  double f =0;
  for(int i=LIy; i<=LSy; i++){
    f=0;
    for(int j=LIx; j<=LSx; j++)
      f+= data[(y+i)*N + j+x]*kernelx[kN+j];
    sum+=f*kernely[kM+i];
    //weight+=kernely[kM+i];
  }

  return (float)sum;//weight!=0? sum/weight:0;
}

inline
void sconvolution(const float *_RESTRICT_ data,
           const float *_RESTRICT_ kernelx,
           const float *_RESTRICT_ kernely,
           float *_RESTRICT_ out,
           const int M, const int N,
           const int KM, const int KN)
{
  const int kM=KM/2;
  const int kN=KN/2;
  for(int y=0; y<M; y++)
    for(int x=0; x<N; x++){
      int LIx = (x-kN)>=0? -kN:-x;
      int LSx = (x+kN)<N?  kN:N-x-1;
      int LIy = (y-kM)>=0? -kM:-y;
      int LSy = (y+kM)<M? kM:M-y-1;

      double sum=0;
      //double weight=0;
      double f =0;
      for(int i=LIy; i<=LSy; i++){
        f=0;
        for(int j=LIx; j<=LSx; j++)
          f+= data[(y+i)*N + j+x]*kernelx[kN+j];
        sum+=f*kernely[kM+i];
        //weight+=kernely[kM+i];
      }
      out[y*N + x]=(float)sum;//sum/weight;
    }
}

void gen_gaborKernel(Eigen::ArrayXXf& greal, Eigen::ArrayXXf& gimag,
                     const double f,
                     const double sigma)
{
  const int N =(int) sigma*3;

  greal.resize(1,2*N+1);
  gimag.resize(1,2*N+1);

  for(int i=-N; i<=N; i++){
    greal(0,i+N)=(float)(exp(-i*i/(2*sigma*sigma))*cos(f*i));
    gimag(0,i+N)=(float)(-exp(-i*i/(2*sigma*sigma))*sin(f*i));
  }
}

void gen_gaussianKernel(Eigen::ArrayXXf& hx, Eigen::ArrayXXf& hy,
                        const double sigma)
{
  const int N =(int) sigma*3;

  hx.resize(1,2*N+1);
  hy.resize(1,2*N+1);

  for(int i=-N; i<=N; i++){
    hx(0,i+N)=(float)exp(-i*i/(2*sigma*sigma));
    hy(0,i+N)=(float)exp(-i*i/(2*sigma*sigma));
  }
}

void gabor_adaptiveFilterXY(const Eigen::ArrayXXf& data,
                            Eigen::ArrayXXf& fr, Eigen::ArrayXXf& fi,
                            const double wx, const double wy,
                            const int x, const int y)
{
  Eigen::ArrayXXf hxr, hxi, hyr, hyi;
  double sx = fabs(wx)>0.001? fabs(1.57/wx):1570,
      sy = fabs(wy)>0.001? fabs(1.57/wy):1570;

  sx = sx>7? 7:(sx<1? 1:sx);
  sy = sy>7? 7:(sy<1? 1:sy);

  gen_gaborKernel(hxr, hxi, wx, sx);
  gen_gaborKernel(hyr, hyi, wy, sy);

  fr(y,x)=convolutionAtXY(data, hxr, hyr, x, y) -
      convolutionAtXY(data, hxi, hyi, x, y);
  fi(y,x)=convolutionAtXY(data, hxr, hyi, x, y) +
      convolutionAtXY(data, hxi, hyr, x, y);
}

void gabor_filter(const Eigen::ArrayXXf& data,
                  Eigen::ArrayXXf& fr, Eigen::ArrayXXf& fi,
                  const double wx, const double wy)
{
  Eigen::ArrayXXf hxr, hxi, hyr, hyi;
  Eigen::ArrayXXf tmp1(data.rows(), data.cols());
  Eigen::ArrayXXf tmp2(data.rows(), data.cols());
  double sx = fabs(1.5708/wx), sy = fabs(1.5708/wy);

  sx = sx>22? 22:(sx<1? 1:sx);
  sy = sy>22? 22:(sy<1? 1:sy);
  gen_gaborKernel(hxr, hxi, wx, sx);
  gen_gaborKernel(hyr, hyi, wy, sy);

  sconvolution(data.data(), hxr.data(), hyr.data(),
               tmp1.data(), data.rows(), data.cols(), hyr.cols(), hxr.cols());
  sconvolution(data.data(), hxi.data(), hyi.data(),
               tmp2.data(), data.rows(), data.cols(), hyi.cols(), hxi.cols());
  fr = tmp1 - tmp2;
  sconvolution(data.data(), hxr.data(), hyi.data(),
               tmp1.data(), data.rows(), data.cols(), hyi.cols(), hxr.cols());
  sconvolution(data.data(), hxi.data(), hyr.data(),
               tmp2.data(), data.rows(), data.cols(), hyr.cols(), hxi.cols());
  fi = tmp1 + tmp2;
}

Eigen::ArrayXXf gaussian_filter(const Eigen::ArrayXXf& data, float sigma)
{
  Eigen::ArrayXXf hx, hy;
  Eigen::ArrayXXf tmp(data.rows(), data.cols());
  gen_gaussianKernel(hx, hy, sigma);

  sconvolution(data.data(), hx.data(), hy.data(),
               tmp.data(), data.rows(), data.cols(), hy.cols(), hx.cols());

  return tmp;
}

Eigen::Array2f peak_freqXY(const Eigen::ArrayXXf& fx,
                           const Eigen::ArrayXXf& fy, Eigen::ArrayXXi& visited,
                           const int x, const int y)
{
  const int N=9;
  Eigen::Array2f freqs;
  freqs << 0,0;
  int cont=0;

  for(int i=y-N/2; i<=y+N/2; i++)
    for(int j=x-N/2; j<=x+N/2; j++){
      if(j>=0 && j<fx.cols() && i>=0 && i<fx.rows())
        if(visited(i,j)){
          freqs(0)+=fx(i,j);
          freqs(1)+=fy(i,j);
          cont++;
        }
    }

  freqs(0)=cont>=0? freqs(0)/cont:0;
  freqs(1)=cont>=0? freqs(1)/cont:0;

  return freqs;

}

gabor::FilterXY::FilterXY(const Eigen::ArrayXXf& dat,
                          Eigen::ArrayXXf& fre, Eigen::ArrayXXf& fim)
:data(dat), fr(fre), fi(fim)
{
  m_kernelN=7;
}

gabor::FilterXY::FilterXY(const gabor::FilterXY& cpy)
:data(cpy.data), fr(cpy.fr), fi(cpy.fi), m_kernelN(cpy.m_kernelN)
{
}

/*
void gabor::FilterXY::operator()(const Eigen::ArrayXXf& dat,
                                 Eigen::ArrayXXf& fre, Eigen::ArrayXXf& fim)
{
  data=dat;
  fr=fre;
  fi=fim;
}
*/

void gabor::FilterXY::operator()(const double wx, const double wy, const int x,
                                 const int y)
{
  double sx = fabs(wx)>0.001? fabs(1.57/wx):1570,
      sy = fabs(wy)>0.001? fabs(1.57/wy):1570;

  sx = sx>m_kernelN? m_kernelN:(sx<1? 1:sx);
  sy = sy>m_kernelN? m_kernelN:(sy<1? 1:sy);

  gen_gaborKernel(hxr, hxi, wx, sx);
  gen_gaborKernel(hyr, hyi, wy, sy);

  fr(y,x)=convolutionAtXY(data, hxr, hyr, x, y) -
      convolutionAtXY(data, hxi, hyi, x, y);
  fi(y,x)=convolutionAtXY(data, hxr, hyi, x, y) +
      convolutionAtXY(data, hxi, hyr, x, y);
}

gabor::FilterXY& gabor::FilterXY::setKernelSize(float size)
{
  m_kernelN=size;
  return *this;
}


gabor::FilterNeighbor::FilterNeighbor(const Eigen::ArrayXXf& param_I,
                                      Eigen::ArrayXXf& param_fr,
                                      Eigen::ArrayXXf& param_fi)
  :m_localFilter(param_I, param_fr, param_fi), M(param_I.rows()),
    N(param_I.cols())
{
}

void gabor::FilterNeighbor::operator()(double wx, double wy, int i, int j)
{
  if(i-1>=0)
    m_localFilter(wx, wy, j, i-1);
  else if(i+1<M)
    m_localFilter(wx, wy, j, i+1);
  if(j-1>=0)
    m_localFilter(wx, wy, j-1, i);
  else if(j+1<N)
    m_localFilter(wx, wy, j+1, i);

  m_localFilter(wx, wy, j, i);
}

gabor::FilterNeighbor& gabor::FilterNeighbor::setKernelSize(float size)
{
  m_localFilter.setKernelSize(size);
  return *this;
}

gabor::DemodPixel::DemodPixel(const Eigen::ArrayXXf& parm_I,
                              Eigen::ArrayXXf& parm_fr,
                              Eigen::ArrayXXf& parm_fi,
                              Eigen::ArrayXXf& parm_fx,
                              Eigen::ArrayXXf& parm_fy,
                              Eigen::ArrayXXi& parm_visited)
:m_filter(parm_I, parm_fr, parm_fi), m_calcfreq(parm_fr, parm_fi),
 fx(parm_fx), fy(parm_fy), visited(parm_visited), m_tau(0.15f),
  m_combFreqs(false), m_combN(7)
{
  m_iters=1;
}

gabor::DemodPixel& gabor::DemodPixel::setKernelSize(const float size)
{
  m_filter.setKernelSize(size);
  return *this;
}

gabor::DemodPixel& gabor::DemodPixel::setTau(const float tau)
{
  m_tau=tau;
  return *this;
}

gabor::DemodPixel& gabor::DemodPixel::setMaxFq(const float w)
{
  m_calcfreq.setMaxFq(w);
  return *this;
}

gabor::DemodPixel& gabor::DemodPixel::setMinFq(const float w)
{
  m_calcfreq.setMinFq(w);
  return *this;
}

void gabor::DemodPixel::operator()(const int i, const int j)
{
  Eigen::Array2f freqs, freq;

  freqs= peak_freqXY(fx, fy, visited, j, i);
  visited(i,j)=1;

  for(int iter=0; iter<m_iters; iter++){
    m_filter(freqs(0), freqs(1), i, j);
    freq = m_calcfreq(j, i);
    freq = (!m_calcfreq.changed())? freq:(Eigen::Array2f() << 0,0).finished();
    freq = m_tau*freq + (1-m_tau)*freqs;
    fx(i,j)=freq(0);
    fy(i,j)=freq(1);
    if(m_combFreqs)
      freq = combFreq(freq,i,j);
  }
}

Eigen::Array2f gabor::DemodPixel::combFreq(Eigen::Array2f& freqs,
				      const int i, const int j)
{
  const int N = m_combN;
  const float p=0.3f;//Probabilidad de cambio

  int cont=0, right=0;
  double sum1=0;
  for(int m=i-N/2; m<=i+N/2; m++)
    for(int n=j-N/2; n<=j+N/2; n++)
      if(n>=0 && n<fx.cols() && m>=0 && m<fx.rows())
        if(visited(m,n)){
          sum1=freqs(0)*fx(m,n) + freqs(1)*fy(m,n);
          cont++;
          right+= (sum1>=0? 1:0);
        }
  float cp = (float)right/(float)cont;
  if((1-cp)>p){
    std::cout<<"Cambiamos frecuencias en ("<<i<<", "<<j<<")"<<std::endl;
    freqs(0)=0;
    freqs(1)=0;
    cont=0;

    for(int m=i-N/2; m<=i+N/2; m++)
      for(int n=j-N/2; n<=j+N/2; n++)
        if(n>=0 && n<fx.cols() && m>=0 && m<fx.rows())
          if(visited(m,n)){
            freqs(0)+=fx(m,n);
            freqs(1)+=fy(m,n);
            cont++;
          }
    freqs(0)/=cont>0? cont:1;
    freqs(1)/=cont>0? cont:1;
  }
  return freqs;
}

gabor::DemodPixel& gabor::DemodPixel::setCombFreqs(bool flag)
{
  m_combFreqs=flag;
  return *this;
}

gabor::DemodPixel& gabor::DemodPixel::setCombNsize(const int Nsize)
{
  m_combN=Nsize;
  return *this;
}

gabor::DemodSeed::DemodSeed(const Eigen::ArrayXXf& parm_I, Eigen::ArrayXXf& parm_fr,
                            Eigen::ArrayXXf& parm_fi,
                            Eigen::ArrayXXf& parm_fx,
                            Eigen::ArrayXXf& parm_fy,
                            Eigen::ArrayXXi& parm_visited)
:DemodPixel(parm_I, parm_fr, parm_fi, parm_fx, parm_fy, parm_visited)
{
  m_iters=1;
}

gabor::DemodPixel& gabor::DemodPixel::setIters(const int iters)
{
  m_iters=iters;
  return *this;
}

void gabor::DemodSeed::operator()(Eigen::Array2f& freqs, const int i, const int j)
{
  for(int iter=0; iter<m_iters; iter++){
    m_filter(freqs(0), freqs(1), i,j);
    freqs = m_calcfreq(j, i);
  }
  fx(i,j)=freqs(0);
  fy(i,j)=freqs(1);
  visited(i,j)=1;
}

gabor::CalcFreqXY::CalcFreqXY(Eigen::ArrayXXf& param_fr,
                              Eigen::ArrayXXf& param_fi)
: fr(param_fr), fi(param_fi)
{
  m_maxf=(float)(M_PI/2.0f);
  m_minf=0.1f;
}

gabor::CalcFreqXY& gabor::CalcFreqXY::setMaxFq(const float w)
{
  m_maxf=w;
  return *this;
}

gabor::CalcFreqXY& gabor::CalcFreqXY::setMinFq(const float w)
{
  m_minf=w;
  return *this;
}

Eigen::Array2f gabor::CalcFreqXY::operator()(const int x, const int y)
{
  Eigen::Array2f freqs;
  float imx = x-1>=0? (fi(y,x)-fi(y,x-1)):
                       (fi(y,x+1)-fi(y,x));
  float rex = x-1>=0? (fr(y,x)-fr(y,x-1)):
                       (fr(y,x+1)-fr(y,x));
  float magn = fr(y,x)*fr(y,x) +
      fi(y,x)*fi(y,x);

  if(magn<0.0001f)
    magn=0.0001f; //Evitar devision entre 0

  freqs[0] = (imx*fr(y,x) - fi(y,x)*rex)/magn;

  imx = y-1>=0? (fi(y,x)-fi(y-1,x)):
                       (fi(y+1,x)-fi(y,x));
  rex = y-1>=0? (fr(y,x)-fr(y-1,x)):
                       (fr(y+1,x)-fr(y,x));

  freqs[1] = (imx*fr(y,x) - fi(y,x)*rex)/magn;
  m_changed=false;

  magn=freqs(0)*freqs(0)+freqs(1)*freqs(1);
  if(magn < m_minf*m_minf){
    magn = m_minf/sqrt(magn);
    freqs(0)=freqs(0)*magn;
    freqs(1)=freqs(1)*magn;
    m_changed=true;
  }
  else if(magn > m_maxf*m_maxf){
    magn = m_maxf/sqrt(magn);
    freqs(0)=freqs(0)*magn;
    freqs(1)=freqs(1)*magn;
    m_changed=true;
  }
  return freqs;
}
bool gabor::CalcFreqXY::changed()
{
  return m_changed;
}

gabor::DemodNeighborhood::DemodNeighborhood(const Eigen::ArrayXXf& param_I,
                                            Eigen::ArrayXXf& param_fr,
                                            Eigen::ArrayXXf& param_fi,
                                            Eigen::ArrayXXf& param_fx,
                                            Eigen::ArrayXXf& param_fy,
                                            Eigen::ArrayXXi& param_visited)
:visit(param_visited),
 m_demodPixel(param_I, param_fr, param_fi, param_fx, param_fy, param_visited)
{
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::
  setKernelSize(const float size)
{
  m_demodPixel.setKernelSize(size);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setMaxFq(const float w)
{
  m_demodPixel.setMaxFq(w);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setMinFq(const float w)
{
  m_demodPixel.setMinFq(w);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setTau(const float tau)
{
  m_demodPixel.setTau(tau);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setIters(const int iters)
{
  m_demodPixel.setIters(iters);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setCombFreqs(bool flag)
{
  m_demodPixel.setCombFreqs(flag);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setCombSize(int size)
{
  m_demodPixel.setCombNsize(size);
  return *this;
}

void gabor::DemodNeighborhood::operator()(const int i, const int j)
{
  //if(!visit(i,j))
    m_demodPixel(i, j);
  if(j-1>=0)
    if(!visit(i,j-1))
      m_demodPixel(i, j-1);
  if(j+1<visit.cols())
    if(!visit(i,j+1))
      m_demodPixel(i, j+1);
  if(i-1>=0)
    if(!visit(i-1,j))
      m_demodPixel(i-1, j);
  if(i+1<visit.rows())
    if(!visit(i+1,j))
      m_demodPixel(i+1, j);
  if(j-1>=0 && i-1>=0)
    if(!visit(i-1,j-1))
      m_demodPixel(i-1, j-1);
  if(j+1<visit.cols() && i-1>=0)
    if(!visit(i-1,j+1))
      m_demodPixel(i-1, j+1);
  if(j+1<visit.cols() && i+1<visit.rows())
    if(!visit(i+1,j+1))
      m_demodPixel(i+1, j+1);
  if(j-1>=0 && i+1<visit.rows())
    if(!visit(i+1,j-1))
      m_demodPixel(i+1, j-1);
}




