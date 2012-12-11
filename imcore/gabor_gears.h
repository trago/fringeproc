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

#ifndef GABOR_GEARS
#define GABOR_GEARS

#include <Eigen/Dense>

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
void gen_gaborKernel(Eigen::ArrayXXf& greal, Eigen::ArrayXXf& gimag,
                     const double f, const double sigma,
                     const int type);

void gen_gaussianKernel(Eigen::ArrayXXf& hx, Eigen::ArrayXXf& hy,
                        const double sigma);

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
void gabor_adaptiveFilterXY(const Eigen::ArrayXXf& data, Eigen::ArrayXXf& fr,
                            Eigen::ArrayXXf& fi, const double wx,
                            const double wy,
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
void gabor_filter(const Eigen::ArrayXXf& data, Eigen::ArrayXXf& fr,
                  Eigen::ArrayXXf& fi,
                  const double wx, const double wy);

Eigen::ArrayXXf gaussian_filter(const Eigen::ArrayXXf& data, float sigma);


Eigen::Array2f peak_freqXY(const Eigen::ArrayXXf& fx, const Eigen::ArrayXXf& fy,
                           const Eigen::ArrayXXi& visited,
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


float convolutionAtXY(const Eigen::ArrayXXf& data,
                      const Eigen::ArrayXXf& kernelx,
                      const Eigen::ArrayXXf& kernely, const int x, const int y);

namespace gabor{
  /**
   * Gabor filter at position (x,y).
   *
   * @author Julio C. Estrada
   */
  class FilterXY{
  public:
    FilterXY(const Eigen::ArrayXXf& data, Eigen::ArrayXXf& fre,
             Eigen::ArrayXXf& fim);
    FilterXY(const FilterXY& cpy);

    //virtual void operator()(const Eigen::ArrayXXf& data, Eigen::ArrayXXf& fre,
    //                        Eigen::ArrayXXf& fim);
    virtual void operator()(const double wx, const double wy,
			    const int x, const int y);
    /**
     * Sets the maximum kernel size.
     *
     * @param size the maximum size
     */
    FilterXY& setKernelSize(float size);
  protected:
    Eigen::ArrayXXf hxr, hxi, hyr, hyi;
    const Eigen::ArrayXXf& data;
    Eigen::ArrayXXf &fr, &fi;
  private:
    /** The maximum kernel size. */
    float m_kernelN;
  };

  /**
   * Filtrates the neighborhood around (i,j) with Gabor filter.
   *
   * It applies the Gabor filter to mage I around pixel (i, j) including itself.
   * It tries to filter neighbors (i-1,j) and (i,j-1) if they are into the image
   * matrix; otherwise it filters (i+1,j) or (i,j+1).
   *
   * @author Julio C. Estrada
   */
  class FilterNeighbor{
  public:
    FilterNeighbor(const Eigen::ArrayXXf& param_I, Eigen::ArrayXXf& param_fr,
                   Eigen::ArrayXXf& param_fi);
    void operator()(double wx, double wy, int i, int j);
    FilterNeighbor& setKernelSize(float size);
  protected:
    FilterXY m_localFilter;
  private:
    const int M, N;
  };

  /**
   * Estimates the local frequency at pixel (x,y).
   *
   * Given the real part and imaginary part of a complex data matrix, it
   * estimates the local frequency at location (x,y) using finite
   * differences.
   */
  class CalcFreqXY{
  public:
    CalcFreqXY(Eigen::ArrayXXf& param_fr,
               Eigen::ArrayXXf& param_fi);

    CalcFreqXY& setMinFq(const float w);
    CalcFreqXY& setMaxFq(const float w);
    Eigen::Array2f operator()(const int x, const int y);
    bool changed();
  protected:
    const Eigen::ArrayXXf &fr, &fi;
  private:
    float m_minf, m_maxf;
    bool m_changed;
  };
  
  /**
   * Applies the gabor filter to a given pixel.
   * 
   * It filters the neighborhood around the given pixel and estimates the
   * local frequency and stores the local frequency.
   */
  class DemodPixel{
  public:
    /**
     * Constructor.
     * 
     * @param parm_I The fringe patern intensity
     * @param parm_fr The real part of the gabor filter output
     * @param parm_fi The imaginary part of the gabor filter output
     * @param parm_fx The estimated local frequencies at x-direction
     * @param parm_fy The estimated local frequencies at y-direction
     * @param parm_visited Indicates with ones the already processed
     * pixels
     */
    DemodPixel(const Eigen::ArrayXXf& parm_I, Eigen::ArrayXXf& parm_fr,
               Eigen::ArrayXXf& parm_fi,
               Eigen::ArrayXXf& parm_fx, Eigen::ArrayXXf& parm_fy,
               Eigen::ArrayXXi& parm_visited);

    void operator()(const int i, const int j);
    DemodPixel& setKernelSize(const float size);
    DemodPixel& setTau(const float tau);
    DemodPixel& setMinFq(const float w);
    DemodPixel& setMaxFq(const float w);
    DemodPixel& setIters(const int iters);
    /**
     * Sets if the estimated frequencies are combed.
     * 
     * @param flag True if the estimated frequencies will be combed,
     * false otherwise. Default is true
     */
    DemodPixel& setCombFreqs(bool flag);
    /**
     * Sets the neighborhood comb size for combing the frequencies.
     * 
     * @param Nsize the comb size. Default is 7
     */
    DemodPixel& setCombNsize(const int Nsize);

  protected:
    Eigen::ArrayXXf &fx, &fy;
    Eigen::ArrayXXi &visited;
    FilterNeighbor m_filter;
    CalcFreqXY m_calcfreq;
    int m_iters;
  private:
    /** Parameter of recursive filter */
    float m_tau;
    /** Indicates if the frequencies are combed*/
    bool m_combFreqs;
    /** The neighborhood size of the combing function */
    char m_combN;

    Eigen::Array2f combFreq(Eigen::Array2f& freqs, const int i, const int j);
  };

  class DemodSeed:public DemodPixel{
  public:
    DemodSeed(const Eigen::ArrayXXf& parm_I, Eigen::ArrayXXf& parm_fr,
              Eigen::ArrayXXf& parm_fi,
              Eigen::ArrayXXf& parm_fx, Eigen::ArrayXXf& parm_fy,
              Eigen::ArrayXXi& parm_visited);
    void operator()(Eigen::Array2f& freqs, const int i, const int j);
  };

  class DemodNeighborhood{
  public:
    DemodNeighborhood(const Eigen::ArrayXXf& param_I, Eigen::ArrayXXf& param_fr,
                      Eigen::ArrayXXf& param_fi, Eigen::ArrayXXf& param_fx,
                      Eigen::ArrayXXf& param_fy,
                      Eigen::ArrayXXi& param_visited);

    DemodNeighborhood& setKernelSize(const float size);
    DemodNeighborhood& setMinFq(const float w);
    DemodNeighborhood& setMaxFq(const float w);
    DemodNeighborhood& setTau(const float tau);
    DemodNeighborhood& setIters(const int iters);
    DemodNeighborhood& setCombFreqs(bool flag);
    DemodNeighborhood& setCombSize(int size);
    void operator()(const int i, const int j);
  protected:
    Eigen::ArrayXXi& visit;
    DemodPixel m_demodPixel;
  };
}

#endif
