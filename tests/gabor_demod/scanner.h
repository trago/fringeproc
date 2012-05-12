#ifndef SCANNER_H
#define SCANNER_H

#include <opencv2/core/core.hpp>
#include <list>
#include <vector>

/**
 * Scans sequentially the matrix pixels folloging the magnitude of the gradient.
 *
 * It recives the differences in x- and y-direction and gives the pixel
 * sequence that scans the data matrix following the magnitude of the vector
 * space build with the components.
 *
 * Here we have the reference to differences in x- and y-direction. It gives
 * the flexibility to update these fields outside the class. For example, we
 * can ask a pixel to objects of this class, process data, update
 * the x- and y-direction fields and in the following pixel asking it will
 * use the updated fields automatically.
 *
 * @author Julio C. Estrada
 */
class Scanner
{
public:
  /**
   * Default constructor.
   */
  Scanner();
  /**
   * Builds the scanner with the given differences in x- and y-direction.
   *
   * @param mat_u the differences in x-direction
   * @param mat_v the differences in y-direction
   */
  Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v);
  /**
   * Builds the scanner with the given differences in x- and y-direction,
   * starting at the given pixel point.
   *
   * @param mat_u the differences in x-direction
   * @param mat_v the differences in y-direction
   * @param pixel the starting pixel point
   */
  Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v, cv::Point pixel);

  /**
   * Determines the next pixel point in the sequence.
   *
   * @return true if there was a next pixel, false in other case.
   */
  bool next();
  /**
   * Gets the actual pixel position.
   *
   * @return the actual pixel point
   */
  cv::Point getPosition();
  /**
   * Sets the starting pixel point.
   *
   * @param pixel the starting pixel point
   */
  void setInitPosition(cv::Point pixel);
  /**
   * Sets the minimum magnitude (frequency magnitude) to scan.
   *
   * The minimum frequency magnitude is the minimum magnitude that will be
   * considered for scanning. That is, it will not visit any pixel whose
   * magnitude is lower than this frequency magnitude.
   *
   * @param freq the minimum frequency magnitude.
   */
  void setFreqMin(double freq);

private:
  /** The frequencies or differences in x-direction */
  cv::Mat_<double> m_matu;
  /** The frequencies or differences in y-direction */
  cv::Mat_<double> m_matv;
  /** Label field that marks whith true the already visited pixels. */
  cv::Mat_<bool> m_visited;
  /** The current pixel in the scanning sequence */
  cv::Point m_pixel;
  /** Stores the pixel sequence */
  std::vector<cv::Point> m_path;
  /** The minimum frequency to scan*/
  double m_freqmin;

  /** Inserts the pixel to the path and marks it as visited*/
  void insertPixelToPath(const cv::Point& pixel);
  /** }
   * Moves to the next pixel in the path.
   *
   * @param magn the minimum frequency magnitud to scan
   */
  bool next(double magn);
  /**
   * @todo In development.
   */
  cv::Point findPixel();
  /**
   * @todo In development.
   */
  bool checkNeighbor(cv::Point pixel);

};

class CompPoints{
public:
  CompPoints(const cv::Mat_<float>& matu, const cv::Mat_<float>& matv)
    :m_matu(matu), m_matv(matv){}

  bool operator()(cv::Point p1, cv::Point p2){
    double m1 = m_matu(p1.y,p1.x)*m_matu(p1.y,p1.x) +
        m_matv(p1.y,p1.x)*m_matv(p1.y,p1.x);
    double m2 = m_matu(p2.y,p1.x)*m_matu(p2.y,p2.x) +
        m_matv(p2.y,p2.x)*m_matv(p2.y,p2.x);

    return m1<=m2;
  }

private:
  const cv::Mat_<float>& m_matu;
  const cv::Mat_<float>& m_matv;
};

#endif // SCANNER_H
