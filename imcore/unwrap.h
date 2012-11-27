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

#ifndef UNWRAP_H
#define UNWRAP_H

#ifndef SWIG
#include <opencv2/core/core.hpp>
#include "scanner.h"
#endif

/**
 * Phase unwrapping system.
 * 
 * Phase unwrapping system that uses an infinite impulse response
 * system. This system scans the phase map following the iso-phase
 * contours from the given wrapped phase. To obtain this unwrapping
 * path, it smooths the wrapped phase. The phase unwrapping system is
 * applyed in a neighborhood of \a NxN around pixel \a (x,y). Each
 * pixel is unwrapped sequentially following the path determined by
 * the smoothed phase.
 * 
 * Actually this phase unwrapping system is a \em low-pass filter and
 * we can control its bandwith with its parameter \c tao. We must
 * choose \c tau between 0 and 1.
 *
 * To use it, make an instance of this class passing the
 * corresponding parameters to the constructor. Ons created the
 * instance of this class, you can set a mask that determines the
 * region of interst using \sa setMask.
 *
 */
class Unwrap{
public:

#ifndef SWIG
  /**
   * Phase unwrapping constructor.
   * 
   * Creates the phase unwrapping system with the given parameters. 
   * @param[in] wphase, is the wrapped phase to process.
   * @param[in] tau, this parameter controls the band width of the
   * system. Its value must be between 0 and 1 for stability reasons.
   * @param[in] smooth, this parameter contols the bandwidth of the
   * gaussian filter used to obtain the scanning path from the given
   * wrapped phase.
   * @param[in] N, the neighborhood size around each pixel that is
   * being processid sequentially.
   */
  Unwrap(Eigen::ArrayXXf_<double> wphase, double tau=0.09, double smooth=9, int N=15);
#endif
  /**
   * The destructor.
   */
  virtual ~Unwrap();

  /**
   * Executes the phase unwrapping system.
   * 
   * Executes the phase unwrapping system and returns until all pixels
   * have been processed.
   */
  void run();
  /**
   * Executes the phase unwrapping system.
   * 
   * Executes the phase unwrapping system and stops until the number
   * of pixels given has been processed. When it stops, it remembers
   * the last pixel processed. Next calls start the
   * processing from the last processed pixel.
   * @param[in] iters, the number of pixel to be processed.
   */
  bool runInteractive(int iters=1);

#ifndef SWIG
  /**
   * Returns the matrix where the unwrapped phase values are stored.
   * 
   * This function returns a reference to the internal matrix where
   * the unwrapped phase is stored.
   * @return the reference to the matrix where the unwrapped phase is
   * stored.
   */
  Eigen::ArrayXXf getOutput();
  /**
   * Returns the wrapped phase used as input.
   * 
   * This function returns a reference to the wrapped phase that is
   * going to be processed.
   * @return the reference to the wrapped phase used as input.
   */
  Eigen::ArrayXXf getInput();
  /**
   * Sets the mask that determines the region of interes.
   * 
   * This mask is a matrix of 8-bit integers marking with ones the
   * region of interest and with zeros the region that is not going to
   * be processed.
   */
  void setMask(Eigen::ArrayXXf mask);

  Eigen::ArrayXXf genPath(double sigma);
#endif

  /** 
   * Sets the \c tao parameter that controls the bandwidth of the
   * system.
   * 
   * @param[in] tao, is the parameter that controls the bandwidth of
   * the system. This should be set to a value between 0 and 1 for
   * stability.
   */
  void setTao(double tao);
  /**
   * Sets the starting pixel to process.
   * 
   * Here we set the starting pixel and once set, the scanning path is
   * obtained. That is, when you set the starting pixel, the scanning
   * sequence is restarted.
   */
  void setPixel(cv::Point pixel);
  /**
   * Process only the given pixel.
   * 
   * 
   */
  void processPixel(cv::Point pixel);
  void filterPhase(double simga);

private:
  Eigen::ArrayXXf_<char> _visited;
  Eigen::ArrayXXf_<char> _mask;
  Eigen::ArrayXXf_<double> _wphase;
  Eigen::ArrayXXf_<double> _uphase;
  Eigen::ArrayXXf_<double> _dx;
  Eigen::ArrayXXf_<double> _dy;
  cv::Point _pixel;
  double _tau;
  double _smooth;
  int _N;

  Scanner* _scanner;

  void takeGradient(cv::Point pixel, const int N);
};


#endif
