#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core/core.hpp>

/**
  Fills the matrix using an parabola.

  The model is as follows:
  \f[
  M(m,n) = A(m^2 + n^2),
  \f]
  where A is the magnitude and M is the matrix. The parabola is centered in
  the center index of the matrix.

  @param mat the matrix
  @param A the manitude.
  */
void parabola(cv::Mat mat, float A) throw(cv::Exception);

/**
  Takes the cosine of the matrix given as angle.

  To each element of the matrix angle is taken the cosine and stored in
  the matrix cc.

  @param angle the values taken as argument of the cosine function
  @param cc the cosine values obtained
  */
void cosine(const cv::Mat angle, cv::Mat& cc) throw(cv::Exception);

/**
  Generates the field X and Y with linear spaced data.

  The range for X is [bx, ex] and the range for Y is [by, ey]. This function
  is usefull to generete the domains X and Y for simulating data from math
  functions.

  @note The generated data fields are of single float point.

  @param X the data field in X
  @param Y the data field in Y
  @param bx the low limit of data in X
  @param ex the high limit of data in X
  @param by the low limit of data in Y
  @param ey the high limit of data in Y
  @param M number of rows for X and Y
  @param N number of columns for X and Y
  */
void linspaced(cv::Mat& X, cv::Mat& Y, const float bx, float ex,
               const float by, float ey,
               const int M, const int N);

/**
 * Generates the peaks function.
 *
 * @param M the number of rows
 * @param N the number of columns
 */
cv::Mat peaks(const int M, const int N);
/**
 * Generates a ramp
 * @param wx the tilt in X
 * @param wy the tilt in Y
 * @param M number of rows
 * @param N number of columns
 */
cv::Mat ramp(float wx, float wy, const int M, const int N);

/**
 * Generates the peaks function with speckle noise.
 */
cv::Mat speckle_peaks(const int M, const int N, const float magn,
                      const int speckle_size);

cv::Mat wphase(const cv::Mat p);
cv::Mat mapRange(const cv::Mat mat, float a, float b);

template<typename T>
cv::Mat cos(const cv::Mat mat) throw(cv::Exception)
{
  if(mat.type()!=CV_32F && mat.type()!=CV_64F){
    cv::Exception e(1000, "Type not supported", "cos",
                    std::string(__FILE__),
                    __LINE__);
    throw(e);
  }
  cv::Mat_<T> cc(mat.rows, mat.cols, mat.type());
  const T*const __restrict__ pt = (T*)mat.data;
  T*const __restrict__ pt_cc=(T*)cc.data;

  for(size_t i=0; i<mat.rows*mat.cols; i++)
    pt_cc[i]=std::cos(pt[i]);

  return cc;
}

template<typename T>
cv::Mat sin(const cv::Mat mat) throw(cv::Exception)
{
  if(mat.type()!=CV_32F && mat.type()!=CV_64F){
    cv::Exception e(1000, "Type not supported", "sin",
                    std::string(__FILE__),
                    __LINE__);
    throw(e);
  }
  cv::Mat_<T> cc(mat.rows, mat.cols, mat.type());
  const T*const __restrict__ pt = (T*)mat.data;
  T*const __restrict__ pt_cc=(T*)cc.data;

  for(size_t i=0; i<mat.rows*mat.cols; i++)
    pt_cc[i]=std::sin(pt[i]);

  return cc;
}

template<typename T>
cv::Mat atan2(const cv::Mat& ss, const cv::Mat& cc) throw(cv::Exception)
{
  if(ss.type()!=CV_32F && ss.type()!=CV_64F){
    cv::Exception e(1000, "Type not supported", "sin",
                    std::string(__FILE__),
                    __LINE__);
    throw(e);
  }
  cv::Mat_<T> ang(ss.rows, ss.cols, ss.type());
  T*const __restrict__ pt = (T*)ang.data;
  const T*const __restrict__ pt_cc=(T*)cc.data;
  const T*const __restrict__ pt_ss=(T*)ss.data;

  for(size_t i=0; i<ang.rows*ang.cols; i++)
    pt[i]=std::atan2(pt_ss[i], pt_cc[i]);

  return ang;
}

#endif // UTILS_H
