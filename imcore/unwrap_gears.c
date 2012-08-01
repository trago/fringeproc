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

#include <stdlib.h>
#include <math.h>
#include "unwrap_gears.h"

float sW(float phase)
{
  float m= 2 * M_PI;
  int n= (phase < 0) ? (phase - M_PI) / m : (phase + M_PI) / m;

  return (phase - m * n);
}

double dW(double phase)
{
  double m= 2 * M_PI;
  int n= (phase < 0) ? (phase - M_PI) / m : (phase + M_PI) / m;

  return (phase - m * n);
}

float sunwrap_pixel(const size_t idx, const int x, const int y,
                    const float *restrict phase, 
		    const char *restrict mask,
                    const float *restrict uphase,
                    const uint8_t *restrict visited, float tao,
                    const size_t M, const size_t N)
{
  float val= 0;
  float grad= 0;
  int ndifs= 0;

  if (y - 1 >= 0)
    if (visited[idx-N] && mask[idx-N]) {
      grad+= sW(phase[idx] - uphase[idx-N]);
      val+= uphase[idx-N];
      ndifs++;
    }
  if (y + 1 < M)
    if (visited[idx+N] && mask[idx+N]) {
      grad+= sW(phase[idx] - uphase[idx+N]);
      val+= uphase[idx+N];
      ndifs++;
    }
  if (x - 1 >= 0)
    if (visited[idx-1] && mask[idx-1]) {
      grad+= sW(phase[idx] - uphase[idx-1]);
      val+= uphase[idx-1];
      ndifs++;
    }
  if (x + 1 < N)
    if (visited[idx+1] && mask[idx+1]) {
      grad+= sW(phase[idx] - uphase[idx+1]);
      val+= uphase[idx+1];
      ndifs++;
    }
  // Diagonals
  //static float dist= sqrt(2.0);
  if (x + 1 < N && y + 1 < M)
    if (visited[idx+N+1] && mask[idx+N+1]) {
      grad+= sW(phase[idx] - uphase[idx+N+1]);
      val+= uphase[idx+N+1];
      ndifs++;
    }
  if (x + 1 < N && y - 1 >= 0)
    if (visited[idx-N+1] && mask[idx-N+1]) {
      grad+= sW(phase[idx] - uphase[idx-N+1]);
      val+= uphase[idx-N+1];
      ndifs++;
    }
  if (x - 1 >= 0 && y + 1 < M)
    if (visited[idx+N-1] && mask[idx+N-1]) {
      grad+= sW(phase[idx] - uphase[idx+N-1]);
      val+= uphase[idx+N-1];
      ndifs++;
    }
  if (x - 1 >= 0 && y - 1 >= 0)
    if (visited[idx-N-1] && mask[idx-N-1]) {
      grad+= sW(phase[idx] - uphase[idx-N-1]);
      val+= uphase[idx-N-1];
      ndifs++;
    }
  //grad/=float(ndifs);
  if (ndifs != 0) {
    val/= (float)ndifs;
    grad/= (float)ndifs;
  } else
    val= phase[idx];

  return val + tao*grad;
}

double dunwrap_pixel(const size_t idx, const int x, const int y,
                     const double *restrict phase,
		     const char *restrict mask,
                     const double *restrict uphase,
                     const uint8_t *restrict visited, double tao,
                     const size_t M, const size_t N)
{
  double val= 0;
  double grad= 0;
  int ndifs= 0;

  if (y - 1 >= 0)
    if (visited[idx-N] && mask[idx-N]) {
      grad+= sW(phase[idx] - uphase[idx-N]);
      val+= uphase[idx-N];
      ndifs++;
    }
  if (y + 1 < M)
    if (visited[idx+N] && mask[idx+N]) {
      grad+= sW(phase[idx] - uphase[idx+N]);
      val+= uphase[idx+N];
      ndifs++;
    }
  if (x - 1 >= 0)
    if (visited[idx-1] && mask[idx-1]) {
      grad+= sW(phase[idx] - uphase[idx-1]);
      val+= uphase[idx-1];
      ndifs++;
    }
  if (x + 1 < N)
    if (visited[idx+1] && mask[idx+1]) {
      grad+= sW(phase[idx] - uphase[idx+1]);
      val+= uphase[idx+1];
      ndifs++;
    }
  // Diagonals
  //static float dist= sqrt(2.0);
  if (x + 1 < N && y + 1 < M)
    if (visited[idx+N+1] && mask[idx+N+1]) {
      grad+= sW(phase[idx] - uphase[idx+N+1]);
      val+= uphase[idx+N+1];
      ndifs++;
    }
  if (x + 1 < N && y - 1 >= 0)
    if (visited[idx-N+1] && mask[idx-N+1]) {
      grad+= sW(phase[idx] - uphase[idx-N+1]);
      val+= uphase[idx-N+1];
      ndifs++;
    }
  if (x - 1 >= 0 && y + 1 < M)
    if (visited[idx+N-1] && mask[idx+N-1]) {
      grad+= sW(phase[idx] - uphase[idx+N-1]);
      val+= uphase[idx+N-1];
      ndifs++;
    }
  if (x - 1 >= 0 && y - 1 >= 0)
    if (visited[idx-N-1] && mask[idx-N-1]) {
      grad+= sW(phase[idx] - uphase[idx-N-1]);
      val+= uphase[idx-N-1];
      ndifs++;
    }
  //grad/=float(ndifs);
  if (ndifs != 0) {
    val/= (float)ndifs;
    grad/= (float)ndifs;
  } else
    val= phase[idx];

  return val + tao*grad;
}
