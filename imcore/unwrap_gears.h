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
  @param the unwrapped phase array
  @param a matrix array that indicates with 1 if the pixel has been already
         unwrapped
  @param tao the parameter of the linear system. It must be less than one and
         greater than 0
  @param M the number of rows
  @param N the number of columns
  */
float sunwrap_pixel(const size_t idx, const int x, const int y,
                    const float *phase,
                    const float *uphase,
                    const uint8_t *visited, float tao,
                    const size_t M, const size_t N);

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
  @param the unwrapped phase array
  @param a matrix array that indicates with 1 if the pixel has been already
         unwrapped
  @param tao the parameter of the linear system. It must be less than one and
         greater than 0
  @param M the number of rows
  @param N the number of columns
  */
double dunwrap_pixel(const size_t idx, const int x, const int y,
                     const double *phase,
                     const double *uphase,
                     const uint8_t *visited, double tao,
                     const size_t M, const size_t N);
#ifdef __cplusplus
}
#endif

#endif // UNWRAP_GEARS_H
