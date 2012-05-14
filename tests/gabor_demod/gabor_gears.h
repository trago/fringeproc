#ifndef GABOR_GEARS
#define GABOR_GEARS

#include <opencv2/core/core.hpp>

#define DEMOD_UNKNOWN_TYPE 1000

/**
 * Generates gabor kernels in 1D.
 *
 * It generates a one-dimensional gabor kernel tuned at frequency @f
 * with sigma @sigma.
 *
 * It resizes the kernel matrixes according to @sigma
 *
 * @param greal [output] The real part of the kernel.
 * @param gimag [output] The imaginary part of the kernel.
 * @param f The tuning frequency.
 * @param sigma The width of the gabor filter.
 * @param type The data type of the output. It can be OpenCV types
 * CV_32F or CV_64F.
 * @throw Exception If the type is not correct.
 */
void gen_gaborKernel(cv::Mat& greal, cv::Mat& gimag, const double f,
             const double sigma, const int type) throw(cv::Exception);

/**
 * Applies the gabor filter at (x,y) and stores the result.
 *
 * It applies the gabor filter using a separated convolution at pixel
 * (x,y) tuned at frequencies (wx, wy). The result is stored in the
 * element (x,y) of the output matrixes.
 *
 * @param data The data matrix that is being filtered.
 * @param fr [output] The real part of the result is stored here.
 * @param fi [output] The imaginary part of the result is stored here.
 * @param wx Tuning frequency at x.
 * @param wy Tuning frequency at y.
 * @param x The x-position.
 * @param y The y-position.
 */
void gabor_adaptiveFilterXY(cv::Mat data, cv::Mat fr, cv::Mat fi,
                            const double wx, const double wy,
                            const int x, const int y);

/**
 * Applies a gabor filter to whole image.
 *
 * @param data The data matrix that is being filtered.
 * @param fr [output] The real part of the filtering.
 * @param fi [output] The imaginary part of the filtering.
 * @param wx Tuning frequency at x.
 * @param wy Tuning frequency at y.
 * @param x The x-position.
 * @param y The y-position.
 */
void gabor_filter(cv::Mat data, cv::Mat fr, cv::Mat fi,
                  const double wx, const double wy);

/**
 * Estimates the local frequency at pixel (x,y).
 *
 * Given the real part and imaginary part of a complex data matrix, it
 * estimates the local frequency at location (x,y) using finite
 * differences.
 *
 * @param fr The real part.
 * @param fi The imaginary part.
 * @param x The x-position.
 * @param y The y-position.
 */
cv::Vec2d calc_freqXY(const cv::Mat fr, const cv::Mat fi,
                      const int x, const int y);

cv::Vec2d peak_freqXY(const cv::Mat fx, const cv::Mat fy, cv::Mat visited,
                      const int x, const int y);

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

namespace gabor{
  /**
   * Gabor filter at position (x,y).
   *
   * @author Julio C. Estrada
   */
  class FilterXY{
  public:
    FilterXY(cv::Mat data, cv::Mat fre, cv::Mat fim);
    void operator()(const double wx, const double wy,
                    const int x, const int y);
  private:
    cv::Mat hxr, hxi, hyr, hyi;
    const cv::Mat data;
    cv::Mat fr, fi;
  };

  class FilterNeighbor{
  public:
    FilterNeighbor(cv::Mat param_I, cv::Mat param_fr, cv::Mat param_fi);
    void operator()(double wx, double wy, int i, int j);
  private:
    const cv::Mat I;
    cv::Mat fr, fi;
    FilterXY m_localFilter;
  };
}

#endif
