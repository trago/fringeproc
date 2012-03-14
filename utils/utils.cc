#include "utils.h"
#include <cmath>


void fill_parabola(cv::Mat mat, float A) throw(Exception)
{
  if((mat.depth()!=CV_32F || mat.depth()!=CV_64F) && mat.channels()!=1){
    Exception ex("In fill_parabola",
                 "The matrix must be float or double with one channel");
    throw(ex);
  }
  const int M=mat.rows, N=mat.cols;
  const float cm=M/2.0, cn=N/2.0;


  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++)
      if(mat.depth()==CV_32F)
        mat.at<float>(i,j)= A*((i-cm)*(i-cm) + (j-cn)*(j-cn));
      else
        mat.at<double>(i,j)= A*((i-cm)*(i-cm) + (j-cn)*(j-cn));
}

void cosine(const cv::Mat angle, cv::Mat& cc) throw(Exception)
{
  if((angle.depth()!=CV_32F || angle.depth()!=CV_64F) && angle.channels()!=1){
    Exception ex("In cosine",
                 "The matrix angle must be float or double with one channel");
    throw(ex);
  }
  if(angle.depth()!=cc.depth() && angle.channels()!=1)
    cc = cv::Mat(angle.size(), CV_MAKETYPE(angle.depth(), 1));
  else if(cc.cols!=angle.cols && cc.rows!=angle.rows)
    cc = cv::Mat(angle.size(), CV_MAKETYPE(angle.depth(), 1));

  const int M=angle.rows, N=angle.cols;
  for(int i=0; i<M; i++)
    for(int j=0; j<N; j++)
      if(angle.depth()==CV_32F)
        cc.at<float>(i,j)=cos(angle.at<float>(i,j));
      else
        cc.at<double>(i,j)=cos(angle.at<double>(i,j));
}
