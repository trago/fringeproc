#include <imcore/scanner.h>
#include <utils/utils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "unwrap.h"
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


void takeGradient(cv::Mat_<double> p, cv::Mat_<double> dx,
                  cv::Mat_<double> dy, cv::Point pixel)
{
  for(int i=pixel.y-1; i<=pixel.y+1; i++)
    for(int j=pixel.x-1; j<=pixel.x+1; j++){
      if(i>=1 && i<p.rows && j>=0 && j<p.cols)
        dy(i,j)=p(i,j)-p(i-1,j);
      if(j>=1 && j<p.cols && i>=0 && i<p.rows)
        dx(i,j)=p(i,j)-p(i,j-1);
      if(j==0 && j<p.cols && i>=0 && i<p.rows)
        dx(i,j)=p(i,j+1)-p(i,j);
      if(i==0 && i<p.rows && j>=0 && j<p.cols)
        dy(i,j)=p(i+1,j)-p(i,j);
    }

}

int main(int argc, char* argv[])
{
  using namespace std;

  cv::Mat wphase;
  cv::Mat uphase;
  cv::Mat visited;
  cv::Mat path, dx, dy;

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
  wphase.create(image.rows, image.cols, CV_64F);
  image.convertTo(wphase, CV_64F);
  cv::normalize(wphase, wphase, M_PI, -M_PI, cv::NORM_MINMAX);
  visited= cv::Mat::zeros(image.rows, image.cols, CV_8U);
  uphase = cv::Mat::zeros(image.rows, image.cols, CV_64F);
  dx = cv::Mat::zeros(image.rows, image.cols, CV_64F);
  dy = cv::Mat::zeros(image.rows, image.cols, CV_64F);
  path = sin<double>(wphase);
  cv::GaussianBlur(path, path, cv::Size(0,0), sigma);
  gradient(path, dx, dy);

  //cv::normalize(path, path, 15*M_PI,0, cv::NORM_MINMAX);
  //path = sin<float>(path);
  //unwrap2D(wphase, uphase, tao, sigma,N);
  cv::Point pixel;
  pixel.x=362;
  pixel.y=201;
  Unwrap unwrap(wphase, tao, sigma, N);
  unwrap.setPixel(pixel);
  uphase = unwrap.getOutput();

  int iter=0;
  unwrap.runInteractive();
  do{
    if(iter++ % 9000 ==0){
      imshow("phase", uphase);
      cv::waitKey(32);
    }
  }while(unwrap.runInteractive());
  std::cout<<"Number of pixels: "<< iter<<std::endl;

  cv::namedWindow("phase");
  cv::namedWindow("wphase");
  cv::namedWindow("path");
  imshow("phase", uphase);
  imshow("wphase", wphase);
  imshow("path", cos<double>(uphase));
  imshow("dx", dx);
  imshow("dy", dy);

  cv::waitKey();
  return 0;
}
