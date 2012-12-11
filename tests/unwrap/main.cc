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
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <Eigen/Dense>
#include <CImg.h>
#include <imcore/unwrap.h>

using namespace std;

inline
void imshow(cimg_library::CImgDisplay& disp, const Eigen::ArrayXXf& im)
{
  const cimg_library::CImg<float> img(im.data(), im.cols(),
                                      im.rows(), 1, 1, true);

  disp.display(img);
}

inline
Eigen::ArrayXXf readFltFile(const char* fname)
{
  ifstream file;
  file.open(fname, ios::in);
  float M=0, N=0;
  float val;
  Eigen::ArrayXXf dat((int)M,(int)N);
  //dat.resize(M,N);

  if(file.is_open()){

    file>>M;
    file>>N;
    dat.resize((int)M,(int)N);

    for(int i=0; i<M; i++)
      for(int j=0; j<N;j++){
        file>>val;
        dat(i,j)=val;
      }
  }

  cout<<"("<<M << ", "<< N<<")"<<endl;
  return dat;
}

inline
void writeFltFile(const Eigen::ArrayXXf& mat, const char* fname)
{
  ofstream file;
  file.open(fname, ios::out);
  int M=mat.rows(), N=mat.cols();
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
  using namespace cimg_library;
  Eigen::ArrayXXf wphase;
  Eigen::ArrayXXf uphase;
  Eigen::ArrayXXi visited;
  Eigen::ArrayXXi mask;
  Eigen::ArrayXXf path, dx, dy;
  Eigen::Array2i pixel;

  std::string mfile, phasefile, outfile;

  cimg_usage("Command line argumments");
  const float tau = cimg_option("-t", 0.2f,
                                "Bandwidth of the unwrapping system");
  const float sigma = cimg_option("-s", 13.0f,
                                  "Smooth power to generate the scanning path");
  const int N = cimg_option("-N", 9, "Window size of the phase unwrapper");
  const int x =cimg_option("-x", 10, "x-position of starting pixel");
  const int y =cimg_option("-y", 10, "y-position of starting pixel");
  mfile = cimg_option("-m", "no_input_file",
                      "File name of the mask to select region of interest");
  phasefile = cimg_option("-i" , "no_input_file",
                          "Wrapped phase to be processed");
  outfile = cimg_option("-o", "output.flt",
                        "Output file where result is saved");

  if(phasefile == "no_input_file"){
    cout<< "You must indicate an input file using option -i"
        << endl;
    return 1;
  }

  if(mfile == "no_input_file"){
    mfile="";
  }

  //Eigen::ArrayXXf image = cv::imread(argv[1], 0);
  Eigen::ArrayXXf image = readFltFile(phasefile.c_str());
  if(image.cols()==0 && image.rows()==0){
    cerr<<"Error: file name "<<phasefile<<" can not be opened." << endl;
    return 1;
  }
  if(mfile != ""){
    CImg<int> cimg_mask(mfile.c_str());
    Eigen::Map<Eigen::ArrayXXi> data(cimg_mask.data(), cimg_mask.height(),
                                     cimg_mask.width());
    if(cimg_mask.is_empty()){
      cerr<<"Error: file name "<<phasefile<<" can not be opened." << endl;
      return 1;
    }
    if(mask.rows()!=image.rows() || mask.cols()!=image.cols()){
      cerr<<"Error: Mask dimensions must match image dimensions." << endl;
      return 1;
    }
    mask = data;
  }
  else
    mask = Eigen::ArrayXXi::Constant(image.rows(), image.cols(), 1);

  pixel(0) = x;
  pixel(1)= y;
  wphase.resize(image.rows(), image.cols());
  float a = image.minCoeff(), b= image.maxCoeff();
  float range = b-a;
  const float pi2 = (float)(2.0*M_PI);
  wphase = pi2*(image-a)/range - (float)M_PI;

  a = (float)mask.minCoeff();
  b = (float)mask.maxCoeff();
  range = b-a;
  if(range != 0)
    mask = (mask-(int)a)/(int)range;

  visited= Eigen::ArrayXXi::Zero(image.rows(), image.cols());
  uphase = Eigen::ArrayXXf::Zero(image.rows(), image.cols());

  Unwrap unwrap(wphase, tau, sigma, N);
  unwrap.setPixel(pixel);
  unwrap.setMask(mask);

  CImgDisplay disp_uphase(wphase.cols(), wphase.rows(), "Uphase");

  unwrap.runInteractive();
  do{
    uphase = unwrap.getOutput();
    imshow(disp_uphase, uphase);
  }while(unwrap.runInteractive(9000));

  /*
  unwrap.setTao(0.05);
  unwrap.setPixel(pixel);
  unwrap.runInteractive();
  do{
    uphase = unwrap.getOutput();
    imshow(disp_uphase, uphase);
  }while(unwrap.runInteractive(9000));
  std::cout<<"Number of pixels: "<< iter<<std::endl;
  */

  uphase = unwrap.getOutput();
  writeFltFile(uphase, outfile.c_str());

  CImgDisplay disp_wphase(wphase.cols(), wphase.rows(), "WPhase");
  CImgDisplay disp_path(wphase.cols(), wphase.rows(), "Path");
  path = unwrap.genPath(sigma);

  imshow(disp_uphase, uphase);
  imshow(disp_wphase, wphase);
  imshow(disp_path, path);

  while(!disp_path.is_closed() && !disp_uphase.is_closed()
        && !disp_wphase.is_closed()){
    disp_uphase.wait_all();
  }

  return 0;
}
