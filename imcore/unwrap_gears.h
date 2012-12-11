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

#ifndef UNWRAP_GEARS_H
#define UNWRAP_GEARS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8_t;

/**
  Takes the modulus 2pi (single precision).

  @param phase the value to take the modulus.
  @author Julio C. Estrada
  */
float sW(float phase);
/**
  Unwraps the given pixel using an IIR filter (single precision).

  This lagorithm recives a matrix array with the wrapped data and a matrix
  array where the unwrapped data is stored. As the matrix arrays are
  a chunk of continuous memory, it receives the index (in continuous)
  of the pixel that is going to be unwrapped and the 2D corresponding
  coordinate. That is:
  \f[
  index = y*N+x,
  \f]
  where \f$ (x,y) \f$ are the cartesian coordinetes, and \f$ N \f$ is the
  number of columns.

  References:
  [1] Miguel A. Navarro, Julio C. Estrada, M. Servin, Juan A. Quiroga, and
      Javier Vargas, "Fast two-dimensional simultaneous phase unwrapping and
      low-pass filtering," Opt. Express 20, 2556-2561 (2012)
      http://www.opticsinfobase.org/oe/abstract.cfm?URI=oe-20-3-2556
  [2] Julio C. Estrada, Manuel Servin, and Juan A. Quiroga,
      "Noise robust linear dynamic system for phase unwrapping
      and smoothing," Opt. Express 19, 5126-5133 (2011)
      http://www.opticsinfobase.org/oe/abstract.cfm?URI=oe-19-6-5126


  @param idx index of the pixel in continuous memory
  @param x the x-coordinate of the pixel
  @param y the y-coordenate of the pixel
  @param phase the wrapped phase array
  @param mask the mask that determines the region of interest
  @param uphase the unwrapped phase array
  @param a matrix array that indicates with 1 if the pixel has been already
         unwrapped
  @param tao the parameter of the linear system. It must be less than one and
         greater than 0
  @param M the number of rows
  @param N the number of columns
  */
float sunwrap_pixel(const size_t idx, const int x, const int y,
                    const float *phase, const int* mask,
                    const float *uphase,
                    const int *visited, float tao,
                    const int M, const int N);

/**
  Takes the modulus 2pi (single precision).

  @param phase the value to take the modulus.
  @author Julio C. Estrada
  */
double dW(double phase);
/**
  Unwraps the given pixel using an IIR filter (single precision).

  This lagorithm recives a matrix array with the wrapped data and a matrix
  array where the unwrapped data is stored. As the matrix arrays are
  a chunk of continuous memory, it receives the index (in continuous)
  of the pixel that is going to be unwrapped and the 2D corresponding
  coordinate. That is:
  \f[
  index = y*N+x,
  \f]
  where \f$ (x,y) \f$ are the cartesian coordinetes, and \f$ N \f$ is the
  number of columns.

  References:
  [1] Miguel A. Navarro, Julio C. Estrada, M. Servin, Juan A. Quiroga, and
      Javier Vargas, "Fast two-dimensional simultaneous phase unwrapping and
      low-pass filtering," Opt. Express 20, 2556-2561 (2012)
      http://www.opticsinfobase.org/oe/abstract.cfm?URI=oe-20-3-2556
  [2] Julio C. Estrada, Manuel Servin, and Juan A. Quiroga,
      "Noise robust linear dynamic system for phase unwrapping
      and smoothing," Opt. Express 19, 5126-5133 (2011)
      http://www.opticsinfobase.org/oe/abstract.cfm?URI=oe-19-6-5126


  @param idx index of the pixel in continuous memory
  @param x the x-coordinate of the pixel
  @param y the y-coordenate of the pixel
  @param phase the wrapped phase array
  @param mask the mask that determines the region of interest
  @param uphase the unwrapped phase array
  @param a matrix array that indicates with 1 if the pixel has been already
         unwrapped
  @param tao the parameter of the linear system. It must be less than one and
         greater than 0
  @param M the number of rows
  @param N the number of columns
  */
double dunwrap_pixel(const size_t idx, const int x, const int y,
                     const double *phase, const int* mask,
                     const double *uphase,
                     const int *visited, double tao,
                     const int M, const int N);
#ifdef __cplusplus
}
#endif

#endif // UNWRAP_GEARS_H
