#include <opencv2/highgui/highgui.hpp>
#include <utils/utils.h>
#include "gabor_gears.h"

int main()
{
  const float wx= .3, wy=.65;
  const int M=456, N=456;
  cv::Mat I = cos<float>(ramp(wx, wy, M, N));
  cv::Mat noise(M,N,CV_32F);
  cv::Mat fr(M,N,CV_32F), fi(M,N,CV_32F);
  cv::Mat hxr, hxi, hyr, hyi;
  cv::Mat h;
  cv::Mat fase;
  cv::Mat tmp;

  cv::randn(noise, 0, 3);
  float sx = 1.57/wx, sy = 1.57/wy;
  sx = sx>22? 22:(sx<1? 1:sx);
  sy = sy>22? 22:(sy<1? 1:sy);
  gen_gaborKernel(hxr, hxi, wx, sx, CV_32F);
  gen_gaborKernel(hyr, hyi, wy, sy, CV_32F);
  I=I+noise;

  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++){
      fr.at<float>(i,j)=convolutionAtXY<float>(I, hxr, hyr, j, i) -
          convolutionAtXY<float>(I, hxi, hyi, j, i);
      fi.at<float>(i,j)=convolutionAtXY<float>(I, hxr, hyi, j, i) +
          convolutionAtXY<float>(I, hxi, hyr, j, i);
      //gabor_adaptiveFilterXY(I, fr, fi, wx, wy, j, i); //This line resumes the previous two lines
    }

  h.create(hyr.cols, hxr.cols, CV_32F);
  for(int i=0; i<hyr.cols; i++)
    for(int j=0; j<hxr.cols; j++)
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
