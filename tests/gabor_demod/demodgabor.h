/*
    Adaptive Gavor filter demodulation.
    Copyright (C) 2012  Julio C. Estrada julio@cio.mx

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef DEMODGABOR_H
#define DEMODGABOR_H

#ifndef SWIG
#include <opencv2/core/core.hpp>
#include "gabor_gears.h"
#endif

class Scanner;

/**
 * Implemments an adaptive Gabor filter guided by the local frequencies.
 *
 * @author Julio C. Estrada
 * @note The data processed is double precision.
 */
class DemodGabor
{
public:
  /**
   * Default constructor.
   */
  DemodGabor();
  /**
   * Builds the filter to process the given image.
   *
   * @param I the image to process
   */

#ifndef SWIG
  DemodGabor(const cv::Mat I);

  /**
   * Returns the real part of the output
   * @return the real part of the output
   */
  cv::Mat getFr();
  /**
   * Returns the imaginary part of the output
   * @return the imaginary part of the output
   */
  cv::Mat getFi();
  /**
   * Returns the estimated local frequencies at x-direction
   * @return the estimated local frequencies
   */
  cv::Mat getWx();
  /**
   * Returns the estimated local frequenciesat y-direction
   * @return the estimated local frequencies
   */
  cv::Mat getWy();
  /**
   * Returns the image being processed.
   * @return the image data
   */
  cv::Mat getInput();

  /**
   * Returns the phase of the arctan between estimated real and imaginary
   * part.
   */
  cv::Mat getPhase();

#endif

  /**
    Resets the internal state of the object.

    This can be usefull when we whant to reinitialize the demodulation
    process.
    */
  void reset();

  /**
   * Sets the minimum local frequency to be processed by the scanner.
   *
   * This is a radial frequency, that is, it is the maximum frequency magnitud
   * that is going to be processed by the filter.
   *
   * @param minf the minimum radial frequency to be processed.
   */
  DemodGabor& setScanMinf(double minf);

  /**
   * Removes the background component from the input data.
   *
   * It removes the DC term or background illumination component.
   *
   * @param sigma is the gaussian smooth applied before removing DC
   */
  void removeDC(double sigma);

  /**
    Sets the number of iterations that the gabor filter is applied in each
    point.

    When the convolution is applied, the local frequencies are estimated.
    Given the nuew local frequencies, we can appliy the Gabor filter againg
    iteratively. Here we say how many iterations we whant to apply in each
    scanned pixel.

    @param iters the number of iterations.
    */
  DemodGabor& setIters(const int iters);
  int getIters() const;
  /**
    Sets the number of iterations for the first pixel to process.

    It is usefull to iterate more than once in the first pixel to refine the
    local frequency estimation in this pixel called the seed pixel.

    @param iters the number of iterations.
    */
  DemodGabor& setSeedIters(const int iters);
  int getSeedIters() const;
  /**
    Sets the kernel size of the gabor filter.

    Actually, this is the maximum size that the gabor filtar can have in both
    directions while it is adapted.
  */
  DemodGabor& setKernelSize(const double size);
  DemodGabor& setMaxfq(const double w);
  DemodGabor& setMinfq(const double w);
  DemodGabor& setTau(const double tau);
  DemodGabor& setStartPixel(const cv::Point pixel);
  DemodGabor& setCombFreqs(const bool comb);
  DemodGabor& setCombSize(const int size);
  DemodGabor& setFreqSeed(const double wx, const double wy);

  double getKernelSize() const;
  double getMaxfq() const;
  double getMinfq() const;
  double getTau() const;
  double getCombSize() const;
  double getWxAtScanPoint(Scanner& scan) const;
  double getWyAtScanPoint(Scanner& scan) const;
  double getWxAtPoint(const cv::Point pixel) const;
  double getWyAtPoint(const cv::Point pixel) const;

  cv::Point getStartPixel();



  /**
   * Executes the filtering operation.
   */
  void run();
  void run(Scanner& scan);

  bool runInteractive(Scanner& scan);
private:
  /** The image matrix being processed */
  cv::Mat_<double> m_I;
  /** The real part of the output */
  cv::Mat_<double> m_fr;
  /** The imaginary part of the output */
  cv::Mat_<double> m_fi;
  /** The obtained frequencies at x-direction */
  cv::Mat_<double> m_fx;
  /** The obtained frequencies at y-direction */
  cv::Mat_<double> m_fy;
  /** The phase is stored here when getPhase() is called */
  cv::Mat_<double> m_phase;
  /** Label field marking the pixels already visited */
  cv::Mat_<uchar> m_visited;
  cv::Point m_startPixel;
  cv::Vec2d m_frequencySeed;

  /** The minimum radial frequency to process */
  double m_scanMinf;
  int m_iters;
  int m_seedIters;
  int m_combSize;
  bool m_combFreqs;
  double m_kernelSize;
  double m_maxfq;
  double m_minfq;
  double m_tau;

};

#endif // DEMODGABOR_H
