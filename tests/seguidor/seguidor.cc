#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <imcore/seguidor.h>
#include <utils/utils.h>
#include <filters/fringeproc.h>

int main()
{
  const int M=228, N=228;
  cv::Mat path(M,N,CV_32FC1);
  cv::Mat visited= cv::Mat::zeros(M,N,CV_32FC1);
  sArray arrPath(path.ptr<float>(), M, N);


  //parabola(path, 0.001);
  path = peaks(M,N)*15;
  cosine(path, path);
  filter_sgaussian(path.ptr<float>(), path.ptr<float>(), 5, N, M);
  Seguidor seguidor(arrPath, 5);
  int i=seguidor.get_r(), j=seguidor.get_c();

  cv::namedWindow("visited");
  cv::imshow("visited", visited);
  int iter=0;
  while(seguidor.siguiente()!=false){
    i=seguidor.get_r();
    j=seguidor.get_c();
    visited.at<float>(i,j)=1;

    if((iter++)%50==0){
      cv::imshow("visited", visited);
      cv::waitKey(20);
    }
  }
  cv::imshow("visited", visited);
  cv::waitKey(20);

  cv::namedWindow("path");
  cv::normalize(path, path, 1, 0, cv::NORM_MINMAX);
  cv::imshow("path", path);

  cv::waitKey();
}

