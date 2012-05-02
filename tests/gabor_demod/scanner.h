#ifndef SCANNER_H
#define SCANNER_H

#include <opencv2/core/core.hpp>
#include <list>
#include <vector>

class Scanner
{
public:
  Scanner();
  Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v);
  Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v, cv::Point pixel);

  bool next();
  cv::Point getPosition();
  void setInitPosition(cv::Point pixel);
  void setFreqMin(double freq);

private:
  cv::Mat_<float> m_matu;
  cv::Mat_<float> m_matv;
  cv::Mat_<bool> m_visited;
  cv::Point m_pixel;
  std::vector<cv::Point> m_path;
  double m_freqmin;

  void insertPixelToPath(const cv::Point& pixel);
  bool next(double magn);
  cv::Point findPixel();
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
