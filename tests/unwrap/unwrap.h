#ifndef UNWRAP_H
#define UNWRAP_H

#include <opencv2/core/core.hpp>
#include <imcore/scanner.h>

/** 
 * Phase unwrapping system. 
 *  
 * This phase unwrapping system uses an Infinite Impulse 
 * Response system. 
 */
class Unwrap{
public:

#ifndef SWIG
  Unwrap(cv::Mat_<double> wphase, double tau=0.09, double smooth=9, int N=15);
#endif
  virtual ~Unwrap();

  void run();
  bool runInteractive();
  
  cv::Mat getOutput();
  cv::Mat getInput();
  void setPixel(cv::Point pixel);
  void processPixel(cv::Point pixel);

private:
  cv::Mat_<char> _visited;
  cv::Mat_<double> _wphase;
  cv::Mat_<double> _uphase;
  cv::Point _pixel;
  double _tau;
  double _smooth;
  int _N;

  Scanner* _scanner;

};


#endif
