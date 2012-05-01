#include "scanner.h"
#include <algorithm>
#include <iostream>

Scanner::Scanner()
{
}

Scanner::Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v)
{
  CV_Assert(mat_u.type()==CV_32F && mat_v.type()==CV_32F);

  m_matu=mat_u;
  m_matv=mat_v;
  m_visited = cv::Mat_<bool>::zeros(mat_u.rows, mat_u.cols);
  m_pixel.x=0;
  m_pixel.y=0;
  insertPixelToPath(m_pixel);
}
Scanner::Scanner(const cv::Mat& mat_u, const cv::Mat& mat_v, cv::Point pixel)
{
  CV_Assert(mat_u.type()==CV_32F && mat_v.type()==CV_32F);

  m_matu=mat_u;
  m_matv=mat_v;
  m_visited = cv::Mat_<bool>::zeros(mat_u.rows, mat_u.cols);
  m_pixel=pixel;
  insertPixelToPath(m_pixel);
}

bool Scanner::next()
{
  cv::Vec<double, 8> magn;
  std::vector<cv::Point> pixel(8,cv::Point(0,0));
  while(!m_path.empty()){
    m_pixel=m_path.back();
    for(int i=0; i<8; i++) magn[i]=-1;
    int x=m_pixel.x, y=m_pixel.y;
    if(x-1>=0)
      if(!m_visited(y,x-1)){
        pixel[0]=cv::Point(x-1,y);
        magn[0]=m_matu(y,x-1)*m_matu(y,x-1) + m_matv(y,x-1)*m_matv(y,x-1);
      }
    if(x+1<m_matu.cols)
      if(!m_visited(y,x+1)){
        pixel[1]=cv::Point(x+1,y);
        magn[1]=m_matu(y,x+1)*m_matu(y,x+1) + m_matv(y,x+1)*m_matv(y,x+1);
      }
    if(y-1>=0)
      if(!m_visited(y-1,x)){
        pixel[2]=cv::Point(x,y-1);
        magn[2]=m_matu(y-1,x)*m_matu(y-1,x) + m_matv(y-1,x)*m_matv(y-1,x);
      }
    if(y+1<m_matu.rows)
      if(!m_visited(y+1,x)){
        pixel[3]=cv::Point(x,y+1);
        magn[3]=m_matu(y+1,x)*m_matu(y+1,x) + m_matv(y+1,x)*m_matv(y+1,x);
      }
    if(x-1>=0 && y-1>=0)
      if(!m_visited(y-1,x-1)){
        pixel[4]=cv::Point(x-1,y-1);
        magn[4]=m_matu(y-1,x-1)*m_matu(y-1,x-1) +
            m_matv(y-1,x-1)*m_matv(y-1,x-1);
      }
    if(x+1<m_matu.cols && y-1>=0)
      if(!m_visited(y-1,x+1)){
        pixel[5]=cv::Point(x+1,y-1);
        magn[5]=m_matu(y-1,x+1)*m_matu(y-1,x+1) +
            m_matv(y-1,x+1)*m_matv(y-1,x+1);
      }
    if(x+1<m_matu.cols && y+1<m_matu.rows)
      if(!m_visited(y+1,x+1)){
        pixel[6]=cv::Point(x+1,y+1);
        magn[6]=m_matu(y+1,x+1)*m_matu(y+1,x+1) +
            m_matv(y+1,x+1)*m_matv(y+1,x+1);
      }
    if(x-1>=0 && y+1<m_matu.rows)
      if(!m_visited(y+1,x-1)){
        pixel[7]=cv::Point(x-1,y+1);
        magn[7]=m_matu(y+1,x-1)*m_matu(y+1,x-1) +
            m_matv(y+1,x-1)*m_matv(y+1,x-1);
      }

    int idx=0;
    double mayor=magn[0];
    bool found=mayor>=0? true:false;
    for(int i=1; i<8; i++)
      if(mayor<=magn[i] && magn[i]>=0){
        mayor=magn[i];
        idx=i;
        found=true;
      }
    if(found){
      m_pixel=pixel[idx];
      insertPixelToPath(m_pixel);
      //CompPoints compara(m_matu, m_matv);
      //m_path.sort(compara);
      //m_pixel=m_path.back();
      return true;
    }
    else
      m_path.pop_back();
    if(m_path.size()%5000==0)
      std::cout<<"List elements: "<<m_path.size()<<std::endl;
  }
  return false;
}
cv::Point2i Scanner::getPosition()
{
  return m_pixel;
}

inline
void Scanner::insertPixelToPath(const cv::Point& pixel)
{
  m_visited(pixel.y, pixel.x)=true;
  m_path.push_back(pixel);
}
