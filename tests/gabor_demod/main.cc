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

#include <iostream>
#include <imcore/scanner.h>
#include <utils/utils.h>
#include <imcore/demodgabor.h>

int main(int argc, char* argv[])
{
  double wx= .7, wy=.7;
  const int M=456, N=456;
  Eigen::ArrayXXf I(M,N,CV_64F);
  Eigen::ArrayXXf phase(M,N,CV_64F);
  Eigen::ArrayXXf fx(Eigen::ArrayXXf::zeros(M,N,CV_64F)), fy(Eigen::ArrayXXf::zeros(M,N,CV_64F)),
      ffx, ffy; // las f's son las frecuencias teoricas y las
                // ff's las estimadas
  Eigen::ArrayXXf noise(M,N,CV_64F);
  Eigen::ArrayXXf fr(Eigen::ArrayXXf::zeros(M,N,CV_64F)), fi(Eigen::ArrayXXf::zeros(M,N,CV_64F)),
      visited;
  Eigen::ArrayXXf path(M,N,CV_64F);
  Eigen::ArrayXXf magn;
  Eigen::ArrayXXf hxr, hxi, hyr, hyi;
  Eigen::ArrayXXf h;
  Eigen::ArrayXXf fase;
  Eigen::ArrayXXf tmp;

  if(argc==1){
    // Genera datos de entrada
    //parabola(phase, 0.0008);
    phase = peaks(M, N)*23;
    //phase=ramp(wx, wy, M, N);
    phase.convertTo(phase, CV_64F);
    I=cos<double>(phase);

    gradient(phase, fx, fy);
    cv::randn(noise, 0, 1.2);
    I=I+noise;
  }
  else{
    I=cv::imread(argv[1], 0);
    I.convertTo(I, CV_64F);
    cv::normalize(I,tmp,1,0,cv::NORM_MINMAX);
    cv::GaussianBlur(tmp, I, cv::Size(0,0), 1.7);
    cv::GaussianBlur(I, tmp, cv::Size(0,0), 17);
    I = I - tmp;
  }


  cv::Vec2d freqs;
  cv::Point p;
  freqs[0]=.7; freqs[1]=.7;
  p.x=I.rows/3; p.y=I.cols/2;

  std::cout<<"Frecuencia teorica local en el punto: ("<<fx.at<double>(p.y,p.x)
           <<", "<<fy.at<double>(p.y,p.x) <<")"<<std::endl;

  int i=p.y, j=p.x, cont=0;

  DemodGabor gabor(I);
  gabor.setIters(1).setKernelSize(7).
        setMaxfq(M_PI/2).setMinfq(0.1).setTau(0.97).setSeedIters(11).
        setScanMinf(.5);
  gabor.setCombFreqs(false).setCombSize(3);
  gabor.setStartPixel(p);
  ffx = gabor.getWx();
  ffy = gabor.getWy();
  fr = gabor.getFr();
  fi = gabor.getFi();
  Scanner scan(ffx, ffy, p);
  scan.setFreqMin(.1);
  scan.updateFreqMin(true);
  cv::Point pixel;

  //gabor.run();

  do{
    // Codigo para mostrar resultados en tiempo real
    if((cont++)%5000==0){
      // Genera kerneles del filtro de gabor
      wx = ffx.at<double>(i,j);
      wy = ffy.at<double>(i,j);
      double sx = fabs(1.5708/wx), sy = fabs(1.5708/wy);
      sx = sx>7? 7:(sx<1? 1:sx);
      sy = sy>7? 7:(sy<1? 1:sy);
      gen_gaborKernel(hxr, hxi, wx, sx, CV_64F);
      gen_gaborKernel(hyr, hyi, wy, sy, CV_64F);
      // Genera la parte imaginaria del filtro de gabor para desplegarlo
      h=Eigen::ArrayXXf::zeros(64,64, CV_64F)-1;
      for(int i=0; i<hyr.cols; i++)
        for(int j=0; j<hxr.cols; j++)
          h.at<double>(i,j)=hxr.at<double>(0,j)*hyi.at<double>(0,i) +
              hxi.at<double>(0,j)*hyr.at<double>(0,i);

      cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
      cv::imshow("real", tmp);
      cv::normalize(h,tmp,1,0,cv::NORM_MINMAX);
      cv::imshow("cos(fase)", tmp);
      cv::waitKey(32);
    }
  }while(gabor.runInteractive(scan));

  // Calculo de la fase de salida
  fase = atan2<double>(fi,fr);
  cv::magnitude(ffx, ffy, magn);
  //cv::threshold(ffx, magn, 0, 1, cv::THRESH_BINARY);

  cv::normalize(I,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("I", tmp);
  cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("real", tmp);
  cv::normalize(fi,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("imag", tmp);
  cv::normalize(fase,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("fase", tmp);
  cv::normalize(ffx,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("ffx", tmp);
  cv::normalize(ffy,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("ffy", tmp);
  cv::normalize(cos<double>(fase),tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("cos(fase)", tmp);
  cv::normalize(magn,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("magn", tmp);

  cv::waitKey(0);

  return 0;
}
