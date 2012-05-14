#include <cmath>
#include "gabor_gears.h"

/**
 */
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

double rowConvolutionXY(cv::Mat_<float> I, cv::Mat_<float> h,
                         const int x, const int y)
{
  const int kM=h.cols/2;
  const int M=I.rows;
  const int N=I.cols;
  //const int LIx = (x-kN)>=0? -kN:-x;
  //const int LSx = (x+kN)<N?  kN:N-x-1;
  const int LIy = (y-kM)>=0? -kM:-y;
  const int LSy = (y+kM)<M? kM:M-y-1;

  double sum=0;
  for(int i=LIy; i<=LSy; i++)
    sum+= I(y+i,x)*h(kM+i);

  return sum;
}

double colConvolutionXY(cv::Mat_<float> I, cv::Mat_<float> h,
                         const int x, const int y)
{
  const int kN=h.cols/2;
  const int kM=h.rows/2;
  const int M=I.rows;
  const int N=I.cols;
  const int LIx = (x-kN)>=0? -kN:-x;
  const int LSx = (x+kN)<N?  kN:N-x-1;
  //const int LIy = (y-kM)>=0? -kM:-y;
  //const int LSy = (y+kM)<M? kM:M-y-1;

  double sum=0;
  for(int i=LIx; i<=LSx; i++)
    sum+= I(y,x+i)*h(kN+i);

  return sum;
}

cv::Vec2d calc_freqXY(const cv::Mat fr, const cv::Mat fi,
                      const int x, const int y)
{
  cv::Vec2d freqs;
  double imx = x-1>=0? (fi.at<double>(y,x)-fi.at<double>(y,x-1)):
                       (fi.at<double>(y,x+1)-fi.at<double>(y,x));
  double rex = x-1>=0? (fr.at<double>(y,x)-fr.at<double>(y,x-1)):
                       (fr.at<double>(y,x+1)-fr.at<double>(y,x));
  double magn = fr.at<double>(y,x)*fr.at<double>(y,x) +
      fi.at<double>(y,x)*fi.at<double>(y,x);

  freqs[0] = (imx*fr.at<double>(y,x) - fi.at<double>(y,x)*rex)/magn;

  imx = y-1>=0? (fi.at<double>(y,x)-fi.at<double>(y-1,x)):
                       (fi.at<double>(y+1,x)-fi.at<double>(y,x));
  rex = y-1>=0? (fr.at<double>(y,x)-fr.at<double>(y-1,x)):
                       (fr.at<double>(y+1,x)-fr.at<double>(y,x));

  freqs[1] = (imx*fr.at<double>(y,x) - fi.at<double>(y,x)*rex)/magn;

  magn=freqs[0]*freqs[0]+freqs[1]*freqs[1];
  if(magn < 0.00005*0.00005){
    magn = 0.00005/sqrt(magn);
    freqs[0]=freqs[0]*magn;
    freqs[1]=freqs[1]*magn;
  }
  else if(magn > 1.57*1.57){
    magn = 1.57/sqrt(magn);
    freqs[0]=freqs[0]*magn;
    freqs[1]=freqs[1]*magn;
  }
  return freqs;
}

cv::Vec2d peak_freqXY(const cv::Mat fx, const cv::Mat fy, cv::Mat visited,
                      const int x, const int y)
{
  cv::Vec2d freqs=0;
  int cont=0;
  if(x-1>=0)
    if(visited.at<char>(y,x-1)){
      freqs[0]+=fx.at<double>(y,x-1);
      freqs[1]+=fy.at<double>(y,x-1);
      cont++;
      //return freqs;
    }
  if(x+1<fx.cols)
    if(visited.at<char>(y,x+1)){
      freqs[0]+=fx.at<double>(y,x+1);
      freqs[1]+=fy.at<double>(y,x+1);
      cont++;
      //return freqs;
    }
  if(y-1>=0)
    if(visited.at<char>(y-1,x)){
      freqs[0]+=fx.at<double>(y-1,x);
      freqs[1]+=fy.at<double>(y-1,x);
      cont++;
      //return freqs;
    }
  if(y+1<fx.rows)
    if(visited.at<char>(y+1,x)){
      freqs[0]+=fx.at<double>(y+1,x);
      freqs[1]+=fy.at<double>(y+1,x);
      cont++;
      //return freqs;
    }
  if(x-1>=0 && y-1>=0)
    if(visited.at<char>(y-1,x-1)){
      freqs[0]+=fx.at<double>(y-1,x-1);
      freqs[1]+=fy.at<double>(y-1,x-1);
      cont++;
      //return freqs;
    }
  if(x+1<fx.cols && y-1>=0)
    if(visited.at<char>(y-1,x+1)){
      freqs[0]+=fx.at<double>(y-1,x+1);
      freqs[1]+=fy.at<double>(y-1,x+1);
      cont++;
      //return freqs;
    }
  if(x+1<fx.cols && y+1<fx.rows)
    if(visited.at<char>(y+1,x+1)){
      freqs[0]+=fx.at<double>(y+1,x+1);
      freqs[1]+=fy.at<double>(y+1,x+1);
      cont++;
      //return freqs;
    }
  if(x-1>=0 && y+1<fx.rows)
    if(visited.at<char>(y+1,x-1)){
      freqs[0]+=fx.at<double>(y+1,x-1);
      freqs[1]+=fy.at<double>(y+1,x-1);
      cont++;
      //return freqs;
    }

  freqs[0]=cont>=0? freqs[0]/cont:0;
  freqs[1]=cont>=0? freqs[1]/cont:0;

  return freqs;

}

gabor::FilterXY::FilterXY(cv::Mat dat, cv::Mat fre, cv::Mat fim)
:data(dat), fr(fre), fi(fim)
{
}

void gabor::FilterXY::operator()(const double wx, const double wy, const int x,
                                 const int y)
{
  double sx = fabs(wx)>0.001? fabs(1.57/wx):1570,
      sy = fabs(wy)>0.001? fabs(1.57/wy):1570;

  sx = sx>8? 8:(sx<1? 1:sx);
  sy = sy>8? 8:(sy<1? 1:sy);

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

gabor::FilterNeighbor::FilterNeighbor(cv::Mat param_I, cv::Mat param_fr,
                                      cv::Mat param_fi)
:I(param_I), fr(param_fr), fi(param_fi),
 m_localFilter(param_I, param_fr, param_fi)
{
}

void gabor::FilterNeighbor::operator()(double wx, double wy, int i, int j)
{
  if(i-1>=0)
    m_localFilter(wx, wy, j, i-1);
  else if(i+1<I.rows)
    m_localFilter(wx, wy, j, i+1);
  if(j-1>=0)
    m_localFilter(wx, wy, j-1, i);
  else if(j+1<I.cols)
    m_localFilter(wx, wy, j+1, i);
  
  m_localFilter(wx, wy, j, i);
}
