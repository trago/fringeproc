#include <cmath>
#include <iostream>
#include "gabor_gears.h"

/**
 */
inline
double dconvolutionAtXY(const double *__restrict data,
               const double *__restrict kernelx,
               const double *__restrict kernely,
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

inline
void dconvolution(const double *__restrict data,
         const double *__restrict kernelx,
         const double *__restrict kernely,
         double *__restrict out,
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

inline
float sconvolutionAtXY(const float *__restrict data,
               const float *__restrict kernelx,
               const float *__restrict kernely,
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

inline
void sconvolution(const float *__restrict data,
           const float *__restrict kernelx,
           const float *__restrict kernely,
           float *__restrict out,
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
      out[y*N + x]=sum;//sum/weight;
    }
}

void gen_gaborKernel(cv::Mat& greal, cv::Mat& gimag, const double f,
             const double sigma, const int type) throw(cv::Exception)
{
  const int N =(int) sigma*3;
  greal.create(1,2*N+1, type);
  gimag.create(1,2*N+1, type);

  if(type==CV_32F)
    for(int i=-N; i<=N; i++){
      greal.at<float>(0,i+N)=exp(-i*i/(2*sigma*sigma))*cos(f*i);
      gimag.at<float>(0,i+N)=-exp(-i*i/(2*sigma*sigma))*sin(f*i);
    }
  else if(type==CV_64F)
    for(int i=-N; i<=N; i++){
      greal.at<double>(0,i+N)=exp(-i*i/(2*sigma*sigma))*cos(f*i);
      gimag.at<double>(0,i+N)=-exp(-i*i/(2*sigma*sigma))*sin(f*i);
    }
  else{
    cv::Exception e(DEMOD_UNKNOWN_TYPE, "Data type not supported",
            "gen_gaborKernel", __FILE__, __LINE__);
    throw(e);
  }
}

void gabor_adaptiveFilterXY(cv::Mat data, cv::Mat fr, cv::Mat fi,
                            const double wx, const double wy,
                            const int x, const int y)
{
  cv::Mat hxr, hxi, hyr, hyi;
  double sx = fabs(wx)>0.001? fabs(1.57/wx):1570,
      sy = fabs(wy)>0.001? fabs(1.57/wy):1570;

  sx = sx>7? 7:(sx<1? 1:sx);
  sy = sy>7? 7:(sy<1? 1:sy);

  gen_gaborKernel(hxr, hxi, wx, sx, data.type());
  gen_gaborKernel(hyr, hyi, wy, sy, data.type());

  if(data.type()==CV_32F){
    fr.at<float>(y,x)=convolutionAtXY<float>(data, hxr, hyr, x, y) -
        convolutionAtXY<float>(data, hxi, hyi, x, y);
    fi.at<float>(y,x)=convolutionAtXY<float>(data, hxr, hyi, x, y) +
        convolutionAtXY<float>(data, hxi, hyr, x, y);
  }
  else if(data.type()==CV_64F){
    fr.at<double>(y,x)=convolutionAtXY<double>(data, hxr, hyr, x, y) -
        convolutionAtXY<double>(data, hxi, hyi, x, y);
    fi.at<double>(y,x)=convolutionAtXY<double>(data, hxr, hyi, x, y) +
        convolutionAtXY<double>(data, hxi, hyr, x, y);
  }
}

void gabor_filter(cv::Mat data, cv::Mat fr, cv::Mat fi,
                  const double wx, const double wy)
{
  cv::Mat hxr, hxi, hyr, hyi;
  cv::Mat tmp1(data.rows, data.cols, data.type());
  cv::Mat tmp2(data.rows, data.cols, data.type());
  double sx = fabs(1.5708/wx), sy = fabs(1.5708/wy);

  sx = sx>22? 22:(sx<1? 1:sx);
  sy = sy>22? 22:(sy<1? 1:sy);
  gen_gaborKernel(hxr, hxi, wx, sx, data.type());
  gen_gaborKernel(hyr, hyi, wy, sy, data.type());

  if(data.type()==CV_32F){
    sconvolution(data.ptr<float>(), hxr.ptr<float>(), hyr.ptr<float>(),
                 tmp1.ptr<float>(), data.rows, data.cols, hyr.cols, hxr.cols);
    sconvolution(data.ptr<float>(), hxi.ptr<float>(), hyi.ptr<float>(),
                 tmp2.ptr<float>(), data.rows, data.cols, hyi.cols, hxi.cols);
    fr = tmp1 - tmp2;
    sconvolution(data.ptr<float>(), hxr.ptr<float>(), hyi.ptr<float>(),
                 tmp1.ptr<float>(), data.rows, data.cols, hyi.cols, hxr.cols);
    sconvolution(data.ptr<float>(), hxi.ptr<float>(), hyr.ptr<float>(),
                 tmp2.ptr<float>(), data.rows, data.cols, hyr.cols, hxi.cols);
    fi = tmp1 + tmp2;
  }
  else if(data.type()==CV_64F){
    dconvolution(data.ptr<double>(), hxr.ptr<double>(), hyr.ptr<double>(),
                 tmp1.ptr<double>(), data.rows, data.cols, hyr.cols, hxr.cols);
    dconvolution(data.ptr<double>(), hxi.ptr<double>(), hyi.ptr<double>(),
                 tmp2.ptr<double>(), data.rows, data.cols, hyi.cols, hxi.cols);
    fr = tmp1 - tmp2;
    dconvolution(data.ptr<double>(), hxr.ptr<double>(), hyi.ptr<double>(),
                 tmp1.ptr<double>(), data.rows, data.cols, hyi.cols, hxr.cols);
    dconvolution(data.ptr<double>(), hxi.ptr<double>(), hyr.ptr<double>(),
                 tmp2.ptr<double>(), data.rows, data.cols, hyr.cols, hxi.cols);
    fi = tmp1 + tmp2;
  }
}

cv::Vec2d peak_freqXY(const cv::Mat fx, const cv::Mat fy, cv::Mat visited,
                      const int x, const int y)
{
  const int N=9;
  cv::Vec2d freqs=0;
  int cont=0;

  for(int i=y-N/2; i<=y+N/2; i++)
    for(int j=x-N/2; j<=x+N/2; j++){
      if(j>=0 && j<fx.cols && i>=0 && i<fx.rows)
        if(visited.at<char>(i,j)){
          freqs[0]+=fx.at<double>(i,j);
          freqs[1]+=fy.at<double>(i,j);
          cont++;
        }
    }

  freqs[0]=cont>=0? freqs[0]/cont:0;
  freqs[1]=cont>=0? freqs[1]/cont:0;

  return freqs;

}

gabor::FilterXY::FilterXY()
{
  m_kernelN=7;
}

gabor::FilterXY::FilterXY(cv::Mat dat, cv::Mat fre, cv::Mat fim)
:data(dat), fr(fre), fi(fim)
{
  m_kernelN=7;
}

gabor::FilterXY::FilterXY(const gabor::FilterXY& cpy)
:data(cpy.data), fr(cpy.fr), fi(cpy.fi), m_kernelN(cpy.m_kernelN)
{
}

void gabor::FilterXY::operator()(cv::Mat dat, cv::Mat fre, cv::Mat fim)
{
  data=dat;
  fr=fre;
  fi=fi;
}

void gabor::FilterXY::operator()(const double wx, const double wy, const int x,
                                 const int y)
{
  double sx = fabs(wx)>0.001? fabs(1.57/wx):1570,
      sy = fabs(wy)>0.001? fabs(1.57/wy):1570;

  sx = sx>m_kernelN? m_kernelN:(sx<1? 1:sx);
  sy = sy>m_kernelN? m_kernelN:(sy<1? 1:sy);

  gen_gaborKernel(hxr, hxi, wx, sx, data.type());
  gen_gaborKernel(hyr, hyi, wy, sy, data.type());

  if(data.type()==CV_32F){
    fr.at<float>(y,x)=convolutionAtXY<float>(data, hxr, hyr, x, y) -
        convolutionAtXY<float>(data, hxi, hyi, x, y);
    fi.at<float>(y,x)=convolutionAtXY<float>(data, hxr, hyi, x, y) +
        convolutionAtXY<float>(data, hxi, hyr, x, y);
  }
  else if(data.type()==CV_64F){
    fr.at<double>(y,x)=convolutionAtXY<double>(data, hxr, hyr, x, y) -
        convolutionAtXY<double>(data, hxi, hyi, x, y);
    fi.at<double>(y,x)=convolutionAtXY<double>(data, hxr, hyi, x, y) +
        convolutionAtXY<double>(data, hxi, hyr, x, y);
  }
}

gabor::FilterXY& gabor::FilterXY::setKernelSize(double size)
{
  m_kernelN=size;
  return *this;
}


gabor::FilterNeighbor::FilterNeighbor(cv::Mat param_I, cv::Mat param_fr,
                                      cv::Mat param_fi)
:m_localFilter(param_I, param_fr, param_fi), M(param_I.rows), N(param_I.cols)
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

gabor::FilterNeighbor& gabor::FilterNeighbor::setKernelSize(double size)
{
  m_localFilter.setKernelSize(size);
  return *this;
}

gabor::DemodPixel::DemodPixel(cv::Mat parm_I, cv::Mat parm_fr,
                              cv::Mat parm_fi, cv::Mat parm_fx,
                              cv::Mat parm_fy, cv::Mat parm_visited)
:m_filter(parm_I, parm_fr, parm_fi), m_calcfreq(parm_fr, parm_fi),
 fx(parm_fx), fy(parm_fy), visited(parm_visited), m_tau(0.15), 
  m_combFreqs(false), m_combN(7)
{
  m_iters=1;
}

gabor::DemodPixel& gabor::DemodPixel::setKernelSize(const double size)
{
  m_filter.setKernelSize(size);
  return *this;
}

gabor::DemodPixel& gabor::DemodPixel::setTau(const double tau)
{
  m_tau=tau;
  return *this;
}

gabor::DemodPixel& gabor::DemodPixel::setMaxFq(const double w)
{
  m_calcfreq.setMaxFq(w);
  return *this;
}

gabor::DemodPixel& gabor::DemodPixel::setMinFq(const double w)
{
  m_calcfreq.setMinFq(w);
  return *this;
}

void gabor::DemodPixel::operator()(const int i, const int j)
{
  cv::Vec2d freqs, freq;

  freqs= peak_freqXY(fx, fy, visited, j, i);

  for(int iter=0; iter<m_iters; iter++){
    m_filter(freqs[0], freqs[1], i, j);
    freq = m_calcfreq(j, i);
    freq = (!m_calcfreq.changed())? freq:0.;
    freq = m_tau*freq + (1-m_tau)*freqs;
    if(m_combFreqs)
      freq = combFreq(freq,i,j);
  }
  fx.at<double>(i,j)=freq[0];
  fy.at<double>(i,j)=freq[1];
  visited.at<char>(i,j)=1;
}

cv::Vec2d gabor::DemodPixel::combFreq(cv::Vec2d freqs, 
				      const int i, const int j)
{
  const int N = m_combN;
  const float p=0.3;//Probabilidad de cambio

  int cont=0, right=0;
  double sum1=0;
  for(int m=i-N/2; m<=i+N/2; m++)
    for(int n=j-N/2; n<=j+N/2; n++)
      if(n>=0 && n<fx.cols && m>=0 && m<fx.rows)
        if(visited.at<char>(m,n)){
          sum1=freqs[0]*fx.at<double>(m,n) + freqs[1]*fy.at<double>(m,n);
          cont++;
          right+= (sum1>=0? 1:0);
        }
  float cp = (float)right/(float)cont;
  if((1-cp)>p){
    std::cout<<"Cambiamos frecuencias en ("<<i<<", "<<j<<")"<<std::endl;
    freqs[0]=0;
    freqs[1]=0;
    cont=0;

    for(int m=i-N/2; m<=i+N/2; m++)
      for(int n=j-N/2; n<=j+N/2; n++)
        if(n>=0 && n<fx.cols && m>=0 && m<fx.rows)
          if(visited.at<char>(m,n)){
            freqs[0]+=fx.at<double>(m,n);
            freqs[1]+=fy.at<double>(m,n);
            cont++;
          }
    freqs[0]/=cont>0? cont:1;
    freqs[1]/=cont>0? cont:1;
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

gabor::DemodSeed::DemodSeed(cv::Mat parm_I, cv::Mat parm_fr, cv::Mat parm_fi,
                            cv::Mat parm_fx, cv::Mat parm_fy, cv::Mat
                            parm_visited)
:DemodPixel(parm_I, parm_fr, parm_fi, parm_fx, parm_fy, parm_visited)
{
  m_iters=1;
}

gabor::DemodPixel& gabor::DemodPixel::setIters(const int iters)
{
  m_iters=iters;
  return *this;
}

void gabor::DemodSeed::operator()(cv::Vec2d freqs, const int i, const int j)
{
  for(int iter=0; iter<m_iters; iter++){
    m_filter(freqs[0], freqs[1], i,j);
    freqs = m_calcfreq(j, i);
  }
  fx.at<double>(i,j)=freqs[0];
  fy.at<double>(i,j)=freqs[1];
  visited.at<char>(i,j)=1;
}

gabor::CalcFreqXY::CalcFreqXY(cv::Mat param_fr, cv::Mat param_fi)
: fr(param_fr), fi(param_fi)
{
  m_maxf=M_PI/2;
  m_minf=0.1;
}

gabor::CalcFreqXY& gabor::CalcFreqXY::setMaxFq(const double w)
{
  m_maxf=w;
  return *this;
}

gabor::CalcFreqXY& gabor::CalcFreqXY::setMinFq(const double w)
{
  m_minf=w;
  return *this;
}

cv::Vec2d gabor::CalcFreqXY::operator()(const int x, const int y)
{
  cv::Vec2d freqs;
  double imx = x-1>=0? (fi.at<double>(y,x)-fi.at<double>(y,x-1)):
                       (fi.at<double>(y,x+1)-fi.at<double>(y,x));
  double rex = x-1>=0? (fr.at<double>(y,x)-fr.at<double>(y,x-1)):
                       (fr.at<double>(y,x+1)-fr.at<double>(y,x));
  double magn = fr.at<double>(y,x)*fr.at<double>(y,x) +
      fi.at<double>(y,x)*fi.at<double>(y,x);

  if(magn<0.0001)
    magn=0.0001; //Evitar devision entre 0

  freqs[0] = (imx*fr.at<double>(y,x) - fi.at<double>(y,x)*rex)/magn;

  imx = y-1>=0? (fi.at<double>(y,x)-fi.at<double>(y-1,x)):
                       (fi.at<double>(y+1,x)-fi.at<double>(y,x));
  rex = y-1>=0? (fr.at<double>(y,x)-fr.at<double>(y-1,x)):
                       (fr.at<double>(y+1,x)-fr.at<double>(y,x));

  freqs[1] = (imx*fr.at<double>(y,x) - fi.at<double>(y,x)*rex)/magn;
  m_changed=false;

  magn=freqs[0]*freqs[0]+freqs[1]*freqs[1];
  if(magn < m_minf*m_minf){
    magn = m_minf/sqrt(magn);
    freqs[0]=freqs[0]*magn;
    freqs[1]=freqs[1]*magn;
    m_changed=true;
  }
  else if(magn > m_maxf*m_maxf){
    magn = m_maxf/sqrt(magn);
    freqs[0]=freqs[0]*magn;
    freqs[1]=freqs[1]*magn;
    m_changed=true;
  }
  return freqs;
}
bool gabor::CalcFreqXY::changed()
{
  return m_changed;
}

gabor::DemodNeighborhood::DemodNeighborhood(cv::Mat param_I, cv::Mat param_fr,
                                            cv::Mat param_fi, cv::Mat param_fx,
                                            cv::Mat param_fy,
                                            cv::Mat param_visited)
:m_demodPixel(param_I, param_fr, param_fi, param_fx, param_fy, param_visited),
 visit(param_visited)
{
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::
  setKernelSize(const double size)
{
  m_demodPixel.setKernelSize(size);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setMaxFq(const double w)
{
  m_demodPixel.setMaxFq(w);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setMinFq(const double w)
{
  m_demodPixel.setMinFq(w);
  return *this;
}

gabor::DemodNeighborhood& gabor::DemodNeighborhood::setTau(const double tau)
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
  if(!visit(i,j))
    m_demodPixel(i, j);
  if(j-1>=0)
    if(!visit(i,j-1))
      m_demodPixel(i, j-1);
  if(j+1<visit.cols)
    if(!visit(i,j+1))
      m_demodPixel(i, j+1);
  if(i-1>=0)
    if(!visit(i-1,j))
      m_demodPixel(i-1, j);
  if(i+1<visit.rows)
    if(!visit(i+1,j))
      m_demodPixel(i+1, j);
  if(j-1>=0 && i-1>=0)
    if(!visit(i-1,j-1))
      m_demodPixel(i-1, j-1);
  if(j+1<visit.cols && i-1>=0)
    if(!visit(i-1,j+1))
      m_demodPixel(i-1, j+1);
  if(j+1<visit.cols && i+1<visit.rows)
    if(!visit(i+1,j+1))
      m_demodPixel(i+1, j+1);
  if(j-1>=0 && i+1<visit.rows)
    if(!visit(i+1,j-1))
      m_demodPixel(i+1, j-1);
}




