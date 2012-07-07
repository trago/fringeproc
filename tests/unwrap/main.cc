#include <imcore/scanner.h>
#include <utils/utils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <imcore/unwrap.h>
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


void takeGradient(cv::Mat_<double> p, cv::Mat_<char> mask, 
		  cv::Mat_<double> dx,
                  cv::Mat_<double> dy, cv::Point pixel)
{
  for(int i=pixel.y-1; i<=pixel.y+1; i++)
    for(int j=pixel.x-1; j<=pixel.x+1; j++){
      if(i>=1 && i<p.rows && j>=0 && j<p.cols)
        dy(i,j)=(p(i,j)-p(i-1,j))*mask(i,j);
      if(j>=1 && j<p.cols && i>=0 && i<p.rows)
        dx(i,j)=(p(i,j)-p(i,j-1))*mask(i,j);
      if(j==0 && j<p.cols && i>=0 && i<p.rows)
        dx(i,j)=(p(i,j+1)-p(i,j))*mask(i,j);
      if(i==0 && i<p.rows && j>=0 && j<p.cols)
        dy(i,j)=(p(i+1,j)-p(i,j))*mask(i,j);
    }

}

int main(int argc, char* argv[])
{
  using namespace std;

  cv::Mat wphase;
  cv::Mat uphase;
  cv::Mat visited;
  cv::Mat mask;
  cv::Mat path, dx, dy;
  float tao, sigma;
  int N;

  if(argc != 6 && argc !=5){
    cout<<"Phase unwrapping method." <<endl
        <<"Usage: "<< argv[0] 
	<<" <image_file> [<mask_file>] <tao> <smooth> <N>"<<endl
        <<"\n<image_file> \tis the file name of the image having the wrapped "
	<<"<mask_file> \tis the mask that determines the region of interest."
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
  if(argc == 5){
    mask = cv::Mat::ones(image.rows, image.cols, CV_8S);
    tao = atof(argv[2]);
    sigma = atof(argv[3]);
    N = atoi(argv[4]);
  }
  else{
    cv::Mat mask = cv::imread(argv[2], 0);
    if(image.empty()){
      cerr<<"I can not read the mask image file." << endl;
      return 1;
    }
    if(mask.rows!=image.rows || mask.cols!=image.cols){
      cerr<<"Image and mask dimensions must match." << endl;
      return 1;
    }
    tao = atof(argv[3]);
    sigma = atof(argv[4]);
    N = atoi(argv[5]);
  }
  wphase.create(image.rows, image.cols, CV_64F);
  image.convertTo(wphase, CV_64F);
  cv::normalize(wphase, wphase, M_PI, -M_PI, cv::NORM_MINMAX);
  mask.convertTo(mask, CV_64F);
  double min, max;
  cv::minMaxLoc(mask, &min, &max);
  if(min!=max)
    cv::normalize(mask, mask, 1, 0, cv::NORM_MINMAX);
  
  visited= cv::Mat::zeros(image.rows, image.cols, CV_8U);
  uphase = cv::Mat::zeros(image.rows, image.cols, CV_64F);
  dx = cv::Mat::zeros(image.rows, image.cols, CV_64F);
  dy = cv::Mat::zeros(image.rows, image.cols, CV_64F);
  path = sin<double>(wphase);
  cv::GaussianBlur(path, path, cv::Size(0,0), sigma);
  gradient(path, dx, dy);
  path = path*mask;
  mask.convertTo(mask, CV_8S);

  //cv::normalize(path, path, 15*M_PI,0, cv::NORM_MINMAX);
  //path = sin<float>(path);
  //unwrap2D(wphase, uphase, tao, sigma,N);
  cv::Point pixel;
  pixel.x=362;
  pixel.y=201;
  Unwrap unwrap(wphase, tao, sigma, N);
  unwrap.setPixel(pixel);
  unwrap.setMask(mask);
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
