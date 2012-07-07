#include <utils/utils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "unwrap.h"
#include "unwrap_gears.h"

inline
void sunwrap_neighborhood(const int ii, const int jj, const cv::Mat& wp,
			  const cv::Mat& mask,
			  cv::Mat& pp, cv::Mat& visited, 
			  float tao, const int N)
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
					mask.ptr<char>(),
					pp.ptr<float>(),
					visited.ptr<uchar>(),
					tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
    else
      for(int j=hig_j-1; j>=low_j; j--){
        pp.at<float>(i,j)=sunwrap_pixel(i*wp.cols+j, j, i,
                                        wp.ptr<float>(),
					mask.ptr<char>(),
                                        pp.ptr<float>(),
                                        visited.ptr<uchar>(),
                                        tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
  }
}

inline
void dunwrap_neighborhood(const int ii, const int jj, const cv::Mat& wp,
			  const cv::Mat& mask,
			  cv::Mat& pp, cv::Mat& visited, 
			  double tao, const int N)
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
					 mask.ptr<char>(),
					 pp.ptr<double>(),
					 visited.ptr<uchar>(),
					 tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
    else
      for(int j=hig_j-1; j>=low_j; j--){
        pp.at<double>(i,j)=dunwrap_pixel(i*wp.cols+j, j, i,
					 wp.ptr<double>(),
					 mask.ptr<char>(),
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
void unwrap2D_engine(cv::Mat wphase, cv::Mat mask, cv::Mat uphase, 
		     double tao, double smooth_path, int n, cv::Point pixel)
{
  const int M=wphase.rows, N=wphase.cols;
  cv::Mat visited = cv::Mat::zeros(M, N, CV_8U);
  cv::Mat path, dx, dy;

  if(wphase.type()==CV_32F)
    path = sin<float>(wphase);
  else
    path = sin<double>(wphase);

  if(smooth_path>0){
    cv::GaussianBlur(path, path, cv::Size(0,0), smooth_path, smooth_path);
    gradient(path, dx, dy);
  }

  Scanner scan(dx, dy, pixel); 
  int i,j, iter=0;
  if(wphase.type()==CV_32F)
    do{
      pixel = scan.getPosition();
      i=pixel.y;
      j=pixel.x;
      sunwrap_neighborhood(i, j, wphase, mask, uphase, visited, tao, n);
    }while(scan.next());
  else
    do{
      i=pixel.y;
      j=pixel.x;
      dunwrap_neighborhood(i, j, wphase, mask, uphase, visited, tao, n);
    }while(scan.next());

}

void unwrap2D(cv::Mat wphase, cv::Mat mask, cv::Mat uphase, double tao,
              double smooth_path, int N, cv::Point pixel) throw(cv::Exception)
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
    unwrap2D_engine<float>(wphase, mask, uphase, tao, smooth_path, N, pixel);
  else
    unwrap2D_engine<double>(wphase, mask, uphase, tao, smooth_path, N, pixel);
}

Unwrap::Unwrap(cv::Mat_<double> wphase, double tau, double smooth, int N)
: _wphase(wphase)
{
  _tau = tau;
  _smooth = smooth;
  _N=N;
  _uphase = cv::Mat_<double>::zeros(_wphase.rows, _wphase.cols);
  _visited = cv::Mat_<char>::zeros(_wphase.rows, _wphase.cols);
  _mask = cv::Mat_<char>::ones(_wphase.rows, _wphase.cols);
  _scanner = NULL;
}

Unwrap::~Unwrap()
{
  if (_scanner != NULL) 
    delete _scanner;
}

void Unwrap::run()
{
  unwrap2D(_wphase, _mask, _uphase, _tau, _smooth, _N, _pixel);
}

bool Unwrap::runInteractive(int iters)
{
  cv::Mat_<double> dx, dy;

  if (_scanner==NULL) {
    cv::Mat path = cos<double>(_wphase);
    cv::GaussianBlur(path, path, cv::Size(0,0), _smooth, _smooth);
    gradient(path, dx, dy);
    _scanner = new Scanner(dx, dy, _pixel);
  }

  int iter=0;
  do{
    _pixel = _scanner->getPosition();
    int i= _pixel.y, j=_pixel.x;
    dunwrap_neighborhood(i, j, _wphase, _mask, _uphase, _visited, _tau, _N);
    _visited(i,j)=1;
  }while(_scanner->next() && (++iter)<iters);

  return iter==iters;
}

void Unwrap::processPixel(cv::Point pixel)
{
  int i=pixel.y, j=pixel.x;
  dunwrap_neighborhood(i, j, _wphase, _mask, _uphase, _visited, _tau, _N);
}


void Unwrap::setPixel(cv::Point pixel)
{
  _pixel=pixel;
}

cv::Mat Unwrap::getOutput()
{
  return _uphase;
}
cv::Mat Unwrap::getInput()
{
  return _wphase;
}

void Unwrap::setMask(cv::Mat mask)
{
  _mask = mask;
}
