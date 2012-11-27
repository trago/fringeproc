/**************************************************************************
Copyright (c) 2012, Julio C. Estrada
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

+ Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

+ Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************/

#include <imcore/scanner.h>
#include <utils/utils.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/program_options.hpp>
#include <imcore/unwrap.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

inline
void imshow(const char* wn, Eigen::ArrayXXf im)
{
  Eigen::ArrayXXf tmp;
  im.convertTo(tmp, CV_32F);
  cv::normalize(tmp, tmp, 1, 0, cv::NORM_MINMAX);
  cv::imshow(wn, tmp);
}

inline
Eigen::ArrayXXf readFltFile(const char* fname)
{
  ifstream file;
  file.open(fname, ios::in);
  float M, N;
  float val;
  file>>M;
  file>>N;

  Eigen::ArrayXXf_<double> dat(M,N);
  for(int i=0; i<M; i++)
    for(int j=0; j<N;j++){
      file>>val;
      dat(i,j)=val;
    }

  cout<<"("<<M << ", "<< N<<")"<<endl;
  return dat;
}

inline
void writeFltFile(Eigen::ArrayXXf_<double> mat, const char* fname)
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
  Eigen::ArrayXXf wphase;
  Eigen::ArrayXXf uphase;
  Eigen::ArrayXXf visited;
  Eigen::ArrayXXf mask;
  Eigen::ArrayXXf path, dx, dy;
  cv::Point pixel;

  namespace po = boost::program_options;
  double tau;
  int N, sigma, x, y;
  std::string mfile, phasefile, outfile;
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "Help message")
      ("tau,t", po::value<double>(&tau)->default_value(0.2),
       "Set the bandwidth of the unwrapping system")
      ("sigma,s", po::value<int>(&sigma)->default_value(13),
       "Smoothing parameter to generate the scanning path")
      ("Nwindow,N", po::value<int>(&N)->default_value(9),
       "Window size of the windowed phase unwrapper")
      ("mask,m", po::value<std::string>(&mfile),
       "Mask file name that selects the region of interst")
      ("input", po::value<std::string>(),
       "The wrapped phase to be processed")
      ("output,o", po::value<std::string>(),
       "Output file where unwrapped phase is stored")
      ("xinit,x", po::value<int>(&x)->default_value(13),
       "Direction 'x' of starting point.")
      ("yinit,y", po::value<int>(&y)->default_value(1),
       "Direction 'y' of starting point.");
  po::positional_options_description p;
  p.add("input", -1);
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);

  if(vm.count("help")){
    cout<<"Windowed phase unwrapping program.\n"<<endl;
    cout<<"Usage: " << argv[0] << " <options> input-file" <<endl;
    cout<<"Copyright (C) 2012, Julio C. Estrada\n"<<endl;
    desc.print(cout);
    cout<<endl;
    cout<<"Example:"<<endl
       <<"  $ "<<argv[0]<<" -N 13 -t 0.03 file.flt"<<endl;
    return 0;
  }
  if(vm.count("input")){
    phasefile = vm["input"].as<string>();
  }
  else{
    cout<<"Usage: " << argv[0] << " <options> input-file" <<endl;
    cout<<"Copyright (C) 2012, Julio C. Estrada\n"<<endl;
    cerr << "Error: Input file required. Use --help" << endl;
    return 1;
  }
  if(vm.count("mask")){
    mfile = vm["mask"].as<string>();
  }
  else
    mfile = "";
  if(vm.count("output")){
    outfile = vm["output"].as<string>();
  }
  else
    outfile = "output.flt";
  N = vm["Nwindow"].as<int>();
  tau = vm["tau"].as<double>();
  sigma = vm["sigma"].as<int>()%2==0?
        vm["sigma"].as<int>()+1:vm["sigma"].as<int>();
  x = vm["xinit"].as<int>();
  y = vm["yinit"].as<int>();

  //Eigen::ArrayXXf image = cv::imread(argv[1], 0);
  Eigen::ArrayXXf image = readFltFile(phasefile.c_str());
  if(image.empty()){
    cerr<<"Error: file name "<<phasefile<<" can not be opened." << endl;
    return 1;
  }
  if(mfile != ""){
    mask = cv::imread(argv[2], 0);
    if(mask.empty()){
      cerr<<"Error: file name "<<phasefile<<" can not be opened." << endl;
      return 1;
    }
    if(mask.rows!=image.rows || mask.cols!=image.cols){
      cerr<<"Error: Mask dimensions must match image dimensions." << endl;
      return 1;
    }
  }
  else
    mask = Eigen::ArrayXXf::ones(image.rows, image.cols, CV_8U);
  cout<<"Phase unwrapping with the following parameters:"<<endl
     <<"   Input file: " << phasefile<<endl
     <<"          tau: "<<tau <<endl
     <<"       Window: "<<N<<"x"<<N<<endl
     <<"        sigma: "<<sigma<<endl
     <<"Init position: ("<<x<<", "<<y<<")"<<endl;
  pixel.x = x;
  pixel.y = y;
  wphase.create(image.rows, image.cols, CV_64F);
  image.convertTo(wphase, CV_64F);
  cv::normalize(wphase, wphase, M_PI, -M_PI, cv::NORM_MINMAX);

  double min, max;
  mask.convertTo(mask, CV_64F);
  cv::minMaxLoc(mask, &min, &max);
  if(min!=max)
    cv::normalize(mask, mask, 1, 0, cv::NORM_MINMAX);

  visited= Eigen::ArrayXXf::zeros(image.rows, image.cols, CV_8U);
  uphase = Eigen::ArrayXXf::zeros(image.rows, image.cols, CV_64F);
  mask.convertTo(mask, CV_8S);

  //cv::normalize(path, path, 15*M_PI,0, cv::NORM_MINMAX);
  //path = sin<float>(path);
  //unwrap2D(wphase, uphase, tao, sigma,N);
  //cv::Point pixel;
  //pixel.x=383;
  //pixel.y=331;
  Unwrap unwrap(wphase, tau, sigma, N);
  unwrap.setPixel(pixel);
  unwrap.setMask(mask);

  uphase = unwrap.getOutput();
  int iter=0;
  unwrap.runInteractive();
  do{
    if(iter++ % 3000 ==0){
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

  writeFltFile(uphase, outfile.c_str());

  imshow("phase", uphase);
  imshow("wphase", wphase);
  imshow("path", atan2<double>(sin<double>(uphase),cos<double>(uphase)));

  cv::waitKey();
  return 0;
}
