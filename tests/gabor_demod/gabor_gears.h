#ifndef GABOR_GEARS
#define GABOR_GEARS

#include <opencv2/core/core.hpp>

#define DEMOD_UNKNOWN_TYPE 1000

void gen_gaborKernel(cv::Mat& greal, cv::Mat& gimag, const double f,
             const double sigma, const int type) throw(cv::Exception);

void gabor_adaptiveFilterXY(cv::Mat data, cv::Mat fr, cv::Mat fi, const double wx,
                    const double wy, const int x, const int y);

double dconvolutionAtXY(const double *__restrict data,
               const double *__restrict kernelx,
               const double *__restrict kernely,
               const int x, const int y,
               const int M, const int N,
               const int kM, const int kN);

void dconvolution(const double *__restrict data,
           const double *__restrict kernelx,
           const double *__restrict kernely,
           double *__restrict out,
           const int M, const int N,
           const int KM, const int KN);

float sconvolutionAtXY(const float *__restrict data,
               const float *__restrict kernelx,
               const float *__restrict kernely,
               const int x, const int y,
               const int M, const int N,
               const int kM, const int kN);

void sconvolution(const float *__restrict data,
           const float *__restrict kernelx,
           const float *__restrict kernely,
           float *__restrict out,
           const int M, const int N,
           const int KM, const int KN);

template <typename T>
T convolutionAtXY(const cv::Mat data, const cv::Mat kernelx,
          const cv::Mat kernely, const int x, const int y)
{
  if(data.type()==CV_32F){
    return sconvolutionAtXY(data.ptr<float>(), kernelx.ptr<float>(),
                kernely.ptr<float>(), x, y,
                data.rows, data.cols, kernely.cols/2,
                kernelx.cols/2);
  }
  else if(data.type()==CV_64F){
    return dconvolutionAtXY(data.ptr<double>(), kernelx.ptr<double>(),
                kernely.ptr<double>(), x, y,
                data.rows, data.cols, kernely.cols/2,
                kernelx.cols/2);
  }
  return 0;
}

#endif
