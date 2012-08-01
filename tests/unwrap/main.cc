#include <imcore/scanner.h>
#include <utils/utils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <imcore/unwrap.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

inline
void imshow(const char* wn, cv::Mat im)
{
  cv::Mat tmp;
  im.convertTo(tmp, CV_32F);
  cv::normalize(tmp, tmp, 1, 0, cv::NORM_MINMAX);
  cv::imshow(wn, tmp);
}


cv::Mat readFltFile(char* fname)
{
  ifstream file;
  file.open(fname, ios::in);
  float M, N;
  float val;
  file>>M;
  file>>N;

  cv::Mat_<double> dat(M,N);
  for(int i=0; i<M; i++)
    for(int j=0; j<N;j++){
      file>>val;
      dat(i,j)=val;
    }

  cout<<"("<<M << ", "<< N<<")"<<endl;
  return dat;
}

void writeFltFile(cv::Mat_<double> mat, char* fname)
{
  ofstream file;
  file.open(fname, ios::out);
  float M=mat.cols, N=mat.rows;
  float val;
  file<<M<<endl;
  file<<N<<endl;

  for(int i=0; i<M; i++)
    for(int j=0; j<N;j++){
      val=mat(i,j);
      file<<val<<endl;
    }
}


int main(int argc, char* argv[])
{
  cv::Mat wphase;
  cv::Mat uphase;
  cv::Mat visited;
  cv::Mat mask;
  cv::Mat path, dx, dy;
  cv::Point pixel;
  float tao, sigma;
  int N;

  if(argc != 8 && argc !=7){
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
	<<"<N> \t\tis the neighborhood size used by the linear system."<<endl
	<<"rows \tthe number of rows."<<endl
	<<"cols \tthe number of columns."<<endl;
    return 1;
  }
  //cv::Mat image = cv::imread(argv[1], 0);
  cv::Mat image = readFltFile(argv[1]);
  if(image.empty()){
    cerr<<"I can not read the image file." << endl;
    return 1;
  }
  if(argc == 7){
    mask = cv::Mat::ones(image.rows, image.cols, CV_8S);
    tao = atof(argv[2]);
    sigma = atof(argv[3]);
    N = atoi(argv[4]);
    pixel.x = atoi(argv[5]);
    pixel.y = atoi(argv[6]);
  }
  else{
    mask = cv::imread(argv[2], 0);
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
    pixel.x = atoi(argv[6]);
    pixel.y = atoi(argv[7]);
  }
  wphase.create(image.rows, image.cols, CV_64F);
  image.convertTo(wphase, CV_64F);
  cv::normalize(wphase, wphase, M_PI, -M_PI, cv::NORM_MINMAX);

  double min, max;
  mask.convertTo(mask, CV_64F);
  cv::minMaxLoc(mask, &min, &max);
  if(min!=max)
    cv::normalize(mask, mask, 1, 0, cv::NORM_MINMAX);

  visited= cv::Mat::zeros(image.rows, image.cols, CV_8U);
  uphase = cv::Mat::zeros(image.rows, image.cols, CV_64F);
  mask.convertTo(mask, CV_8S);

  //cv::normalize(path, path, 15*M_PI,0, cv::NORM_MINMAX);
  //path = sin<float>(path);
  //unwrap2D(wphase, uphase, tao, sigma,N);
  //cv::Point pixel;
  //pixel.x=383;
  //pixel.y=331;
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

  /*
  unwrap.setPixel(pixel);
  unwrap.runInteractive();
  unwrap.setTao(0.05);
  iter=0;
  do{
    if(iter++ % 9000 ==0){
      imshow("phase", uphase);
      cv::waitKey(32);
    }
  }while(unwrap.runInteractive());
  */
  std::cout<<"Number of pixels: "<< iter<<std::endl;

  writeFltFile(uphase, "uphase.flt");

  cv::namedWindow("phase");
  cv::namedWindow("wphase");
  cv::namedWindow("path");
  imshow("phase", uphase);
  imshow("wphase", wphase);
  imshow("path", atan2<double>(sin<double>(uphase),cos<double>(uphase)));

  cv::waitKey();
  return 0;
}
