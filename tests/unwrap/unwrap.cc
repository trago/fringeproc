#include <utils/utils.h>
#include "unwrap.h"
#include "unwrap_gears.h"

inline
void sunwrap_neighborhood(const int ii, const int jj, const cv::Mat& wp,
                         cv::Mat& pp, cv::Mat& visited, float tao, const int N)
{
  int low_i = (ii-N/2)>=0? (ii-N/2):0;
  int hig_i = (ii+N/2)<(wp.rows-1)? (ii+N/2):(wp.rows-1);
  int low_j = (jj-N/2)>=0? (jj-N/2):0;
  int hig_j = (jj+N/2)<(wp.cols-1)? (jj+N/2):(wp.cols-1);

  for(int i=low_i; i<hig_i; i++){
    if(i%2==0)
      for(int j=low_j; j<hig_j; j++){
        pp.at<float>(i,j)=sunwrap_pixel(i*wp.cols+j, j, i,
                                           wp.ptr<float>(),
                                           pp.ptr<float>(),
                                           visited.ptr<uchar>(),
                                           tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
    else
      for(int j=hig_j-1; j>=low_j; j--){
        pp.at<float>(i,j)=sunwrap_pixel(i*wp.cols+j, j, i,
                                        wp.ptr<float>(),
                                        pp.ptr<float>(),
                                        visited.ptr<uchar>(),
                                        tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
  }
}

inline
void dunwrap_neighborhood(const int ii, const int jj, const cv::Mat& wp,
                         cv::Mat& pp, cv::Mat& visited, double tao, const int N)
{
  int low_i = (ii-N/2)>=0? (ii-N/2):0;
  int hig_i = (ii+N/2)<(wp.rows-1)? (ii+N/2):(wp.rows-1);
  int low_j = (jj-N/2)>=0? (jj-N/2):0;
  int hig_j = (jj+N/2)<(wp.cols-1)? (jj+N/2):(wp.cols-1);

  for(int i=low_i; i<hig_i; i++){
    if(i%2==0)
      for(int j=low_j; j<hig_j; j++){
        pp.at<double>(i,j)=dunwrap_pixel(i*wp.cols+j, j, i,
                                           wp.ptr<double>(),
                                           pp.ptr<double>(),
                                           visited.ptr<uchar>(),
                                           tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
    else
      for(int j=hig_j-1; j>=low_j; j--){
        pp.at<double>(i,j)=dunwrap_pixel(i*wp.cols+j, j, i,
                                        wp.ptr<double>(),
                                        pp.ptr<double>(),
                                        visited.ptr<uchar>(),
                                        tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
  }
}

/** 
 * Phase unwrapping method 
 */
template<typename T>
void unwrap2D_engine(cv::Mat wphase, cv::Mat uphase, double tao,
                     double smooth_path, int n, cv::Point pixel)
{
  const int M=wphase.rows, N=wphase.cols;
  cv::Mat visited = cv::Mat::zeros(M, N, CV_8U);
  cv::Mat path, dx, dy;

  if(wphase.type()==CV_32F)
    path = sin<float>(wphase);
  else
    sin<double>(wphase).convertTo(path, CV_32F);

  if(smooth_path>0){
    gradient(path, dx, dy);
    cv::GaussianBlur(dx, dx, cv::Size(0,0), smooth_path, smooth_path);
    cv::GaussianBlur(dy, dy, cv::Size(0,0), smooth_path, smooth_path);
  }

  Scanner scan(dx, dy, pixel); 
  int i,j, iter=0;
  if(wphase.type()==CV_32F)
    do{
      pixel = scan.getPosition();
      i=pixel.y;
      j=pixel.x;
      sunwrap_neighborhood(i, j, wphase, uphase, visited, tao, n);
    }while(scan.next());
  else
    do{
      i=pixel.y;
      j=pixel.x;
      dunwrap_neighborhood(i, j, wphase, uphase, visited, tao, n);
    }while(scan.next());

}

void unwrap2D(cv::Mat wphase, cv::Mat uphase, double tao,
              double smooth_path=0.0, int N=7) throw(cv::Exception)
{
  if(wphase.type()!=CV_32F && wphase.type()!=CV_64F){
    cv::Exception e(1000,
                    "Type not supported, must be single or double precision.",
                    "unwrap2D", std::string(__FILE__), __LINE__);
    throw(e);
  }
  if(uphase.rows!=wphase.rows && uphase.cols!=wphase.cols &&
     uphase.type()!=wphase.type())
    uphase.create(wphase.rows, wphase.cols, wphase.type());

  if(wphase.type()==CV_32F)
    unwrap2D_engine<float>(wphase, uphase, tao, smooth_path, N);
  else
    unwrap2D_engine<double>(wphase, uphase, tao, smooth_path, N);
}

Unwrap::Unwrap(cv::Mat_<double> wphase, double tau, double smooth, int N)
: _wphase(wphase)
{
  _tau = tau;
  _smooth = smooth;
  _N=N;
  _uphase = cv::Mat_<double>::zeros(_wphase.rows, _wphase.cols);
  _visited = cv::Mat_<char>::zeros(_wphase.rows, _wphase.cols);
  _scanner = NULL;
}

Unwrap::~Unwrap()
{
  if (_scanner != NULL) 
    delete _scanner;
}

void Unwrap::run()
{
  unwrap2D_engine(_wphase, _uphase, _tau, _smooth, _N, _pixel);
}

bool Unwrap::runInteractive()
{
  cv::Mat_<double> dx, dy;
  path = cos<double>(_wphase);
  cv::GaussianBlur(path, path, cv::Size(0,0), _smooth);
  gradient(path, dx, dy);

  if (_scanner==NULL) 
    _scanner = new Scanner(dx, dy, pixel);

  pixel = _scanner->getPosition();
  int i= pixel.y, j=pixel.x;
  dunwrap_neighborhood(i, j, _wphase, _uphase, _visited, _tao, _N);

  return _scanner->next();
}

