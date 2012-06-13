#include "utils.h"
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>

void gradient(const cv::Mat I, cv::Mat& dx, cv::Mat& dy)
{
  if((I.depth()!=CV_32F || I.depth()!=CV_64F) && I.channels()!=1){
    cv::Exception e(1000, "The matrix must be float or double with channel=1",
                    "parabola", std::string(__FILE__), __LINE__);
    throw(e);
  }
  dx.create(I.rows, I.cols, I.type());
  dy.create(I.rows, I.cols, I.type());

  for(int i=0; i<I.rows-1; i++)
    for(int j=0; j<I.cols-1; j++){
      if(I.depth()==CV_32F){
	dx.at<float>(i,j) = I.at<float>(i,j+1)-I.at<float>(i,j);
	dy.at<float>(i,j) = I.at<float>(i+1,j)-I.at<float>(i,j);
      }
      else{
	dx.at<double>(i,j) = I.at<double>(i,j+1)-I.at<double>(i,j);
	dy.at<double>(i,j) = I.at<double>(i+1,j)-I.at<double>(i,j);
      }
    }
  for(int i=0; i<I.rows; i++)
    if(I.depth()==CV_32F)
      dy.at<float>(i,I.cols-1)=dy.at<float>(i,I.cols-2);
    else
      dy.at<double>(i,I.cols-1)=dy.at<double>(i,I.cols-2);
  for(int i=0; i<I.cols; i++)
    if(I.depth()==CV_32F)
      dx.at<float>(I.rows-1,i)=dx.at<float>(I.rows-2,i);
    else
      dx.at<double>(I.rows-1,i)=dx.at<double>(I.rows-2,i);
}

void parabola(cv::Mat mat, float A) throw(cv::Exception)
{
  if((mat.depth()!=CV_32F || mat.depth()!=CV_64F) && mat.channels()!=1){
    cv::Exception e(1000, "The matrix must be float or double with channel=1",
                    "parabola", std::string(__FILE__), __LINE__);
    throw(e);
  }
  const int M=mat.rows, N=mat.cols;
  const float cm=M/2.0, cn=N/2.0;


  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++)
      if(mat.depth()==CV_32F)
        mat.at<float>(i,j)= A*((i-cm)*(i-cm) + (j-cn)*(j-cn));
      else
        mat.at<double>(i,j)= A*((i-cm)*(i-cm) + (j-cn)*(j-cn));
}

void cosine(const cv::Mat angle, cv::Mat& cc) throw(cv::Exception)
{
  if((angle.depth()!=CV_32F || angle.depth()!=CV_64F) && angle.channels()!=1){
    cv::Exception e(1000, "The matrix must be float or double with channel=1",
                    "cosine", std::string(__FILE__), __LINE__);
    throw(e);
  }
  if(angle.depth()!=cc.depth() && angle.channels()!=1)
    cc = cv::Mat(angle.size(), CV_MAKETYPE(angle.depth(), 1));
  else if(cc.cols!=angle.cols && cc.rows!=angle.rows)
    cc = cv::Mat(angle.size(), CV_MAKETYPE(angle.depth(), 1));

  const int M=angle.rows, N=angle.cols;
  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++)
      if(angle.depth()==CV_32F)
        cc.at<float>(i,j)=cos(angle.at<float>(i,j));
      else
        cc.at<double>(i,j)=cos(angle.at<double>(i,j));
}

void linspaced(cv::Mat& X, cv::Mat& Y, const float bx, float ex,
               const float by, float ey,
               const int M, const int N)
{
  X = cv::Mat(M,N,CV_32F);
  Y = cv::Mat(M,N,CV_32F);
  const float stepx = (ex-bx)/(float)(N-1);
  const float stepy = (ey-by)/(float)(M-1);
  float *const __restrict__ ptx=(float*)X.data;
  float *const __restrict__ pty=(float*)Y.data;
  size_t idx;

  float x=bx;
  float y=by;
  for(int i=0; i<M; i++){
    idx=N*i;
    x=bx;
    for(int j=0; j<N; j++){
      ptx[idx+j]=x;
      pty[idx+j]=y;
      x+=stepx;
    }
    y+=stepy;
  }
}

cv::Mat peaks(const int M, const int N)
{
  cv::Mat X, Y;
  cv::Mat p(M,N, CV_32F);
  cv::Mat tmp;
  linspaced(X, Y, -3.0, 3.0, -3.0, 3.0, M, N);

  p = 1 - X/2.0;
  cv::pow(X,5,tmp);
  p+= tmp;
  cv::pow(Y,3,tmp);
  p+= tmp;
  cv::multiply(X,X,X);
  cv::multiply(Y,Y,Y);
  cv::exp(-X-Y,tmp);
  cv::multiply(p,tmp,p);

  return p;
}

cv::Mat ramp(float wx, float wy, const int M, const int N)
{
  cv::Mat X, Y, res;
  linspaced(X, Y, 0, M-1, 0, N-1, M, N);
  res = wx*X + wy*Y;

  return res;
}

cv::Mat speckle_peaks(const int M, const int N, const float magn,
               const int speckle_size)
{
  cv::Mat Ic0;
  cv::Mat Ic1;
  cv::Mat p = peaks(M,N)*magn;
  cv::Mat noise0(M,N,CV_32F);
  cv::Mat noise1(M,N,CV_32F);
  const float pi=M_PI;

  cv::randu(noise0, cv::Vec<float,1>(-1), cv::Vec<float,1>(1));
  noise0*=pi;
  cv::randu(noise1, cv::Vec<float,1>(-1), cv::Vec<float,1>(1));
  noise1*=pi;
  cv::blur(noise0, noise0, cv::Size(speckle_size,speckle_size));
  cv::blur(noise1, noise1, cv::Size(speckle_size,speckle_size));
  noise0 = mapRange(noise0, -pi, pi);
  noise1 = mapRange(noise1, -pi, pi);

  Ic0 = wphase(noise0);
  Ic1 = wphase(noise1 + p + pi/2.);

  return wphase(Ic0-Ic1);
}

cv::Mat wphase(const cv::Mat p)
{
  cv::Mat wp(p.rows, p.cols, CV_32F);
  float *const __restrict__ pt_wp = (float*)wp.data;
  float *const __restrict__ pt_p = (float*)p.data;
  size_t idx;

  for(int i=0; i<p.rows; i++){
    idx=i*p.cols;
    for(int j=0; j<p.cols; j++)
      pt_wp[idx+j] = atan2(sin(pt_p[idx+j]), cos(pt_p[idx+j]));
  }

  return wp;
}

cv::Mat mapRange(const cv::Mat mat, float a, float b)
{
  cv::Mat img;
  double min;
  double max;

  cv::minMaxLoc(mat, &min, &max);

  float m = (b-a)/(max-min);

  img = m*(mat-min) + a;
  return img;
}
