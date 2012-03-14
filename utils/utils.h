#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core/core.hpp>
#include <imcore/exception.h>

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
void parabola(cv::Mat mat, float A) throw(Exception);

/**
  Takes the cosine of the matrix given as angle.

  To each element of the matrix angle is taken the cosine and stored in
  the matrix cc.

  @param angle the values taken as argument of the cosine function
  @param cc the cosine values obtained
  */
void cosine(const cv::Mat angle, cv::Mat& cc) throw(Exception);

#endif // UTILS_H
