#ifndef FREQSCAN_H
#define FREQSCAN_H

#include "scanner.h"
#include <list>

/**
 * This scanns an image following a frequency map
 */
class FreqScan: public Scanner
{
public:
  FreqScan(int rows, int cols);
  FreqScan(const FreqScan& other);
  ~FreqScan();
  FreqScan& operator=(const FreqScan& other);
  
  cv::Point getPosition();
  
  /**
   * Builds the scanner starting at the given pixel point.
   *
   * @param pixel the starting pixel point
   */
  FreqScan(cv::Point pixel, float wx, float wy, int rows, int cols);
  /**
   * Determines the next pixel point in the sequence.
   *
   * @return true if there was a next pixel, false in other case.
   */
  virtual bool next();
  
  void setDirection(float wx, float wy);
  
private:
    /** The direction to scan */
  cv::Mat_<double> m_angle;
    /** Label field that marks whith true the already visited pixels. */
  cv::Mat_<bool> m_visited;
  /** The current pixel in the scanning sequence */
  cv::Point m_pixel;
  /** Stores the pixel sequence */
  std::list<cv::Point> m_path;
  
  void give_next(double dir);
  bool validate_next();
  
};

#endif // FREQSCAN_H
