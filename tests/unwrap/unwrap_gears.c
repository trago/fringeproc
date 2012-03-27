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
                    const float *restrict uphase,
                    const uint8_t *restrict visited, float tao,
                    const size_t M, const size_t N)
{
  float val= 0;
  float grad= 0;
  int ndifs= 0;

  if (y - 1 >= 0)
    if (visited[idx-N]) {
      grad+= sW(phase[idx] - uphase[idx-N]);
      val+= uphase[idx-N];
      ndifs++;
    }
  if (y + 1 < M)
    if (visited[idx+N]) {
      grad+= sW(phase[idx] - uphase[idx+N]);
      val+= uphase[idx+N];
      ndifs++;
    }
  if (x - 1 >= 0)
    if (visited[idx-1]) {
      grad+= sW(phase[idx] - uphase[idx-1]);
      val+= uphase[idx-1];
      ndifs++;
    }
  if (x + 1 < N)
    if (visited[idx+1]) {
      grad+= sW(phase[idx] - uphase[idx+1]);
      val+= uphase[idx+1];
      ndifs++;
    }
  // Diagonals
  //static float dist= sqrt(2.0);
  if (x + 1 < N && y + 1 < M)
    if (visited[idx+N+1]) {
      grad+= sW(phase[idx] - uphase[idx+N+1]);
      val+= uphase[idx+N+1];
      ndifs++;
    }
  if (x + 1 < N && y - 1 >= 0)
    if (visited[idx-N+1]) {
      grad+= sW(phase[idx] - uphase[idx-N+1]);
      val+= uphase[idx-N+1];
      ndifs++;
    }
  if (x - 1 >= 0 && y + 1 < M)
    if (visited[idx+N-1]) {
      grad+= sW(phase[idx] - uphase[idx+N-1]);
      val+= uphase[idx+N-1];
      ndifs++;
    }
  if (x - 1 >= 0 && y - 1 >= 0)
    if (visited[idx-N-1]) {
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
                     const double *phase,
                     const double *uphase,
                     const uint8_t *visited, double tao,
                     const size_t M, const size_t N)
{
  double val= 0;
  double grad= 0;
  int ndifs= 0;

  if (y - 1 >= 0)
    if (visited[idx-N]) {
      grad+= sW(phase[idx] - uphase[idx-N]);
      val+= uphase[idx-N];
      ndifs++;
    }
  if (y + 1 < M)
    if (visited[idx+N]) {
      grad+= sW(phase[idx] - uphase[idx+N]);
      val+= uphase[idx+N];
      ndifs++;
    }
  if (x - 1 >= 0)
    if (visited[idx-1]) {
      grad+= sW(phase[idx] - uphase[idx-1]);
      val+= uphase[idx-1];
      ndifs++;
    }
  if (x + 1 < N)
    if (visited[idx+1]) {
      grad+= sW(phase[idx] - uphase[idx+1]);
      val+= uphase[idx+1];
      ndifs++;
    }
  // Diagonals
  //static float dist= sqrt(2.0);
  if (x + 1 < N && y + 1 < M)
    if (visited[idx+N+1]) {
      grad+= sW(phase[idx] - uphase[idx+N+1]);
      val+= uphase[idx+N+1];
      ndifs++;
    }
  if (x + 1 < N && y - 1 >= 0)
    if (visited[idx-N+1]) {
      grad+= sW(phase[idx] - uphase[idx-N+1]);
      val+= uphase[idx-N+1];
      ndifs++;
    }
  if (x - 1 >= 0 && y + 1 < M)
    if (visited[idx+N-1]) {
      grad+= sW(phase[idx] - uphase[idx+N-1]);
      val+= uphase[idx+N-1];
      ndifs++;
    }
  if (x - 1 >= 0 && y - 1 >= 0)
    if (visited[idx-N-1]) {
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
