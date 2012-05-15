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

#include <opencv2/core/core.hpp>
#include "gabor_gears.h"

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
   * Sets the minimum local frequency to be processed.
   *
   * This is a radial frequency, that is, it is the maximum frequency magnitud
   * that is going to be processed by the filter.
   *
   * @param minf the minimum radial frequency to be processed.
   */
  void setMinf(double minf);

  /**
   * Removes the background component from the input data.
   *
   * It removes the DC term or background illumination component.
   */
  void removeDC();

  /**
   * Executes the filtering operation.
   */
  void run();
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
  /** Label field marking the pixels already visited */
  cv::Mat_<uchar> m_visited;
  
  /** The minimum radial frequency to process */
  double m_minf;
  
};

#endif // DEMODGABOR_H
