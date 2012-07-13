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
  Unwrap(cv::Mat_<double> wphase, double tau=0.09, double smooth=9, int N=15);
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
   * the last pixel processed. Next calls processed start the
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
  cv::Mat getOutput();
  /**
   * Returns the wrapped phase used as input.
   * 
   * This function returns a reference to the wrapped phase that is
   * going to be processed.
   * @return the reference to the wrapped phase used as input.
   */
  cv::Mat getInput();
  /**
   * Sets the mask that determines the region of interes.
   * 
   * This mask is a matrix of 8-bit integers marking with ones the
   * region of interest and with zeros the region that is not going to
   * be processed.
   */
  void setMask(cv::Mat mask);

  cv::Mat genPath(double sigma);
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
  void processPixel(cv::Point pixel);
  void filterPhase(double simga);
  cv::Mat genPath(double sigma);

private:
  cv::Mat_<char> _visited;
  cv::Mat_<char> _mask;
  cv::Mat_<double> _wphase;
  cv::Mat_<double> _uphase;
  cv::Mat_<double> _dx;
  cv::Mat_<double> _dy;
  cv::Point _pixel;
  double _tau;
  double _smooth;
  int _N;

  Scanner* _scanner;

  void takeGradient(cv::Point pixel, const int N);
};


#endif
