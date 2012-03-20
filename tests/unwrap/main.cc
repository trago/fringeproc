#include <imcore/seguidor.h>
#include <utils/utils.h>
#include <opencv2/highgui/highgui.hpp>
#include "unwrap_gears.h"
#include <iostream>

inline
void imshow(const char* wn, cv::Mat im)
{
  cv::Mat tmp;
  cv::normalize(im, tmp, 1, 0, cv::NORM_MINMAX);
  cv::imshow(wn, tmp);
}

inline
void unwrap_neighborhood(const int ii, const int jj, const cv::Mat& wp,
                         cv::Mat& pp, cv::Mat& visited, float tao, const int N)
{
  int low_i = (ii-N/2)>=0? (ii-N/2):0;
  int hig_i = (ii+N/2)<(wp.rows-1)? (ii+N/2):(wp.rows-1);
  int low_j = (jj-N/2)>=0? (jj-N/2):0;
  int hig_j = (jj+N/2)<(wp.cols-1)? (jj+N/2):(wp.cols-1);

  for(int i=low_i; i<hig_i; i++){
    if(i%2==0)
      for(int j=low_j; j<hig_j; j++){
        pp.at<float>(i,j)=calcUnwrapped(i*wp.cols+j, j, i,
                                           wp.ptr<float>(),
                                           pp.ptr<float>(),
                                           visited.ptr<uchar>(),
                                           tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
    else
      for(int j=hig_j-1; j>=low_j; j--){
        pp.at<float>(i,j)=calcUnwrapped(i*wp.cols+j, j, i,
                                        wp.ptr<float>(),
                                        pp.ptr<float>(),
                                        visited.ptr<uchar>(),
                                        tao, wp.rows, wp.cols);
        visited.at<uchar>(i,j)=1;
      }
  }
}

int main()
{
  const int M=355, N=355;
  cv::Mat phase = peaks(M,N)*20;
  cv::Mat wphase = atan2<float>(sin<float>(phase), cos<float>(phase));
  cv::Mat uphase = cv::Mat::zeros(M, N, CV_32F);
  cv::Mat visited= cv::Mat::zeros(M,N, CV_8U);

  Seguidor scan(sin<float>(wphase), 128);
  int i,j, iter=0;
  float tao=0.7;
  do{
    i=scan.get_r();
    j=scan.get_c();
    //uphase.at<float>(i,j)=calcUnwrapped(i*N+j, j, i,
    //                                    wphase.ptr<float>(),
    //                                    uphase.ptr<float>(),
    //                                    visited.ptr<uchar>(),
    //                                    tao, M, N);
    //visited.at<uchar>(i,j)=1;
    unwrap_neighborhood(i, j, wphase, uphase, visited, tao, 5);
    if(iter++ % 1000 ==0){
      imshow("phase", uphase);
      cv::waitKey(32);
    }
  }while(scan.siguiente());
  std::cout<<"Number of pixels: "<< iter<<std::endl;
  cv::namedWindow("phase");
  cv::namedWindow("wphase");
  imshow("phase", wphase);
  imshow("wphase", uphase);

  cv::waitKey();
  return 0;
}
