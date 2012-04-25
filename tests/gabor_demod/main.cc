#include <opencv2/highgui/highgui.hpp>
#include <utils/utils.h>
#include "gabor_gears.h"

int main()
{
  const float wx= .1, wy=.1;
  const int M=256, N=256;
  cv::Mat I = cos<float>(ramp(wx, wy, M, N));
  cv::Mat fr(M,N,CV_32F), fi(M,N,CV_32F);
  cv::Mat hxr, hxi, hyr, hyi;
  cv::Mat h;
  cv::Mat fase;
  cv::Mat tmp;

  gen_gaborKernel(hxr, hxi, wx, 25, CV_32F);
  gen_gaborKernel(hyr, hyi, wy, 25, CV_32F);
 
  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++){
      fr.at<float>(i,j)=convolutionAtXY<float>(I, hxr, hyr, j, i) - 
	convolutionAtXY<float>(I, hxi, hyi, j, i);
      fi.at<float>(i,j)=convolutionAtXY<float>(I, hxr, hyi, j, i) +
	convolutionAtXY<float>(I, hxi, hyr, j, i);
    }

  h.create(hxr.cols, hxr.cols, CV_32F);
  for(int i=0; i<hxr.cols; i++)
    for(int j=0; j<hyr.cols; j++)
      h.at<float>(i,j)=hxr.at<float>(0,j)*hyr.at<float>(0,i) - 
	hxi.at<float>(0,j)*hyi.at<float>(0,i);

  fase = atan2<float>(fi,fr);

  cv::namedWindow("I");
  cv::namedWindow("real");
  cv::namedWindow("imag");
  cv::namedWindow("kernel");
  cv::namedWindow("fase");

  cv::normalize(I,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("I", tmp);
  cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("real", tmp);
  cv::normalize(fi,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("imag", tmp);
  cv::normalize(h,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("kernel", tmp);
  cv::normalize(fase,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("fase", tmp);

  cv::waitKey(0);

  return 0;
}
