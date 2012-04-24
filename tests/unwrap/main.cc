#include <imcore/seguidor.h>
#include <utils/utils.h>
#include <filters/fringeproc.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "unwrap_gears.h"
#include <iostream>
#include <cstdlib>

inline
void imshow(const char* wn, cv::Mat im)
{
  cv::Mat tmp;
  im.convertTo(tmp, CV_32F);
  cv::normalize(tmp, tmp, 1, 0, cv::NORM_MINMAX);
  cv::imshow(wn, tmp);
}

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

template<typename T>
void unwrap2D_engine(cv::Mat wphase, cv::Mat uphase, double tao,
                double smooth_path, int n)
{
  const int M=wphase.rows, N=wphase.cols;
  cv::Mat visited = cv::Mat::zeros(M, N, CV_8U);
  cv::Mat path;

  if(wphase.type()==CV_32F)
    path = sin<float>(wphase);
  else
    sin<double>(wphase).convertTo(path, CV_32F);

  if(smooth_path>0)
    cv::GaussianBlur(path, path, cv::Size(0,0), smooth_path, smooth_path);

  Seguidor scan(path, 28); //Discretizes the dynamic rango in 128 levels
  int i,j, iter=0;
  if(wphase.type()==CV_32F)
    do{
      i=scan.get_r();
      j=scan.get_c();
      sunwrap_neighborhood(i, j, wphase, uphase, visited, tao, n);
    }while(scan.siguiente());
  else
    do{
      i=scan.get_r();
      j=scan.get_c();
      dunwrap_neighborhood(i, j, wphase, uphase, visited, tao, n);
    }while(scan.siguiente());

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

int main(int argc, char* argv[])
{
  using namespace std;

  cv::Mat wphase;
  cv::Mat uphase;
  cv::Mat visited;
  cv::Mat path;

  if(argc != 5){
    cout<<"Phase unwrapping method." <<endl
        <<"Usage: "<< argv[0] <<" <image_file> <tao> <smooth> <N>"<<endl
        <<"\n<image_file> \tis the file name of the image having the wrapped "
        <<"phase map."<<endl
        <<"<tao> \t\tis the paramater of the linear low-pass filter. You mus "
        <<"\n\t\tchoose tao>0 and tao<1"<<endl
        <<"<smooth> \tThe scanning path is taken from the phase map. This "
        <<"parameter \n\t\tis to remove noise from the path."<<endl
        <<"<N> \t\tis the neighborhood size used by the linear system."<<endl;
    return 1;
  }
  cv::Mat image = cv::imread(argv[1], 0);
  if(image.empty()){
    cerr<<"I can not read the image file." << endl;
    return 1;
  }
  float tao = atof(argv[2]);
  float sigma = atof(argv[3]);
  int N = atoi(argv[4]);
  wphase.create(image.rows, image.cols, CV_32F);
  image.convertTo(wphase, CV_32F);
  cv::normalize(wphase, wphase, M_PI, -M_PI, cv::NORM_MINMAX);
  visited= cv::Mat::zeros(image.rows, image.cols, CV_8U);
  uphase = cv::Mat::zeros(image.rows, image.cols, CV_32F);
  path = sin<float>(wphase);
  cv::GaussianBlur(path, path, cv::Size(0,0), sigma, sigma);

  //cv::normalize(path, path, 15*M_PI,0, cv::NORM_MINMAX);
  //path = sin<float>(path);
  //unwrap2D(wphase, uphase, tao, sigma,N);
  Seguidor scan(path, 28);
  int i,j, iter=0;
  do{
    i=scan.get_r();
    j=scan.get_c();
    sunwrap_neighborhood(i, j, wphase, uphase, visited, tao, N);
    //uphase.at<float>(i,j)=10;
    if(iter++ % 9000 ==0){
      imshow("phase", uphase);
      cv::waitKey(32);
    }
  }while(scan.siguiente());
  std::cout<<"Number of pixels: "<< iter<<std::endl;

  cv::namedWindow("phase");
  cv::namedWindow("wphase");
  cv::namedWindow("path");
  imshow("phase", uphase);
  imshow("wphase", wphase);
  imshow("path", cos<float>(uphase));

  cv::waitKey();
  return 0;
}
