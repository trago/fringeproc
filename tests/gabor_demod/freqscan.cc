#include "freqscan.h"
#include <cmath>


FreqScan::FreqScan(int rows, int cols)
{
  m_pixel.x=0;
  m_pixel.y=0;
  m_visited = cv::Mat_<bool>::zeros(rows, cols);
  m_angle =  cv::Mat_<double>::zeros(rows, cols);
  m_path.push_front(m_pixel);
}

FreqScan::FreqScan(cv::Point pixel, float wx, float wy, int rows, int cols)
{
  m_pixel = pixel;
  m_path.push_front(m_pixel);
  m_visited = cv::Mat_<bool>::zeros(rows, cols);
  m_angle =  cv::Mat_<double>::zeros(rows, cols);

  m_angle.at<double>(pixel.y,pixel.x) = atan2(wy,wx);
}

FreqScan::FreqScan(const FreqScan& other)
{
  m_visited = other.m_visited;
  m_angle = other.m_angle;
  m_pixel = other.m_pixel;
  m_path = other.m_path;
}

FreqScan::~FreqScan()
{

}

FreqScan& FreqScan::operator=(const FreqScan& other)
{   
  m_visited = other.m_visited;
  m_angle = other.m_angle;
  m_pixel = other.m_pixel;
  m_path = other.m_path;  
}

cv::Point FreqScan::getPosition()
{
  return m_pixel;
}


bool FreqScan::next()
{
  m_pixel = m_path.front();
  double angle = m_angle.at<double>(m_pixel.y, m_pixel.x);
  
  give_next(angle);
  while(!validate_next());
  m_visited.at<bool>(m_pixel.y, m_pixel.x)=true;
  
  return !m_path.empty();
}

bool FreqScan::validate_next()
{
  const int m = m_pixel.y;
  const int n = m_pixel.x;
  const int M = m_angle.rows;
  const int N = m_angle.cols;

  if(m_path.empty()){
    m_pixel.x=0;
    m_pixel.y=0;
    return true;
  }
  
  if(m>=M || n>=N || n<0 || m<0){
    m_path.pop_front();
    m_pixel = m_path.front();
    return false;
  }
  if(m_visited.at<bool>(m,n)){
    m_path.pop_front();
    m_pixel = m_path.front();
    bool found = false;
    for(int i=m-1; i<=m+1; i++){
      for(int j=n-1; j<=n+1; j++){
        if(!(i>=M || j>=N || i<0 || j<0)){
          if(!m_visited.at<bool>(i,j)){
            m_pixel.x = j;
            m_pixel.y = i;
            m_path.push_front(m_pixel);
            found = true;
            break;
          }
        }
      }
      if(found)
        break;
    }
    
    return found;
  }
  return true;
}

void FreqScan::give_next(double dir)
{
  const double step = 3.14159265358979323846/4.0; // PI/2
  const double PI = 3.14159265358979323846;
  
  double best_distance = fabs(dir);
  double this_direction = 0;
  int npixel =0;
  double distance = 0;
  cv::Point pixel;
  pixel.x = m_pixel.x+1;
  pixel.y = m_pixel.y;
  
  for(double angle = 0; angle<PI+0.01; angle+=step){
    distance = fabs(angle-dir);
    if(distance<best_distance){
      best_distance = distance;
      this_direction = angle;
    }
  }
  for(double angle = -step; angle>-PI-0.01; angle-=step){
    distance = fabs(angle-dir);
    if(distance<best_distance){
      best_distance = distance;
      this_direction = angle;
    }
  }
  
  if(fabs(this_direction) < 0.001){
    pixel.x = m_pixel.x+1;
    pixel.y = m_pixel.y;
  }
  else if(fabs(this_direction-PI/4.) < 0.001){
    pixel.x = m_pixel.x+1;
    pixel.y = m_pixel.y-1;
  }
  else if(fabs(this_direction-PI/2.) < 0.001){
    pixel.x = m_pixel.x;
    pixel.y = m_pixel.y-1;
  }
  else if(fabs(this_direction-3*PI/4.) < 0.001){
    pixel.x = m_pixel.x-1;
    pixel.y = m_pixel.y-1;
  }
  else if(fabs(this_direction-PI) < 0.001){
    pixel.x = m_pixel.x-1;
    pixel.y = m_pixel.y;
  }
  else if(fabs(this_direction+PI/4.) < 0.001){
    pixel.x = m_pixel.x+1;
    pixel.y = m_pixel.y+1;
  }
  else if(fabs(this_direction+PI/2.) < 0.001){
    pixel.x = m_pixel.x;
    pixel.y = m_pixel.y+1;
  }
  else if(fabs(this_direction+3*PI/4.) < 0.001){
    pixel.x = m_pixel.x-1;
    pixel.y = m_pixel.y+1;
  }
  
  m_path.push_front(pixel);
  m_pixel = pixel;
}

void FreqScan::setDirection(float wx, float wy)
{

  const double magn = sqrt(wx*wx + wy*wy);
  const double angle = (magn < 0.001)? 0:atan2(wx,wy);

  m_angle.at<double>(m_pixel.y, m_pixel.x)= angle;
}
