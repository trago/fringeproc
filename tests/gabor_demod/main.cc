#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <imcore/seguidor.h>
#include "scanner.h"
#include <utils/utils.h>
#include <iostream>
#include "gabor_gears.h"
#include "demodgabor.h"

/**
 * Calculates the gradient differences.
 *
 * @param I is the image.
 * @param dx [output] the differences along x-direction (columns)
 * @param dy [output] the differences along y-direction (rows)
 */
void gradient(const cv::Mat I, cv::Mat& dx, cv::Mat& dy)
{
  dx.create(I.rows, I.cols, I.type());
  dy.create(I.rows, I.cols, I.type());

  for(int i=0; i<I.rows-1; i++)
    for(int j=0; j<I.cols-1; j++){
      dx.at<double>(i,j) = I.at<double>(i,j+1)-I.at<double>(i,j);
      dy.at<double>(i,j) = I.at<double>(i+1,j)-I.at<double>(i,j);
    }
  for(int i=0; i<I.rows; i++)
    dy.at<double>(i,I.cols-1)=dy.at<double>(i,I.cols-2);
  for(int i=0; i<I.cols; i++)
    dx.at<double>(I.rows-1,i)=dx.at<double>(I.rows-2,i);
}

int main(int argc, char* argv[])
{
  double wx= .05, wy=.0;
  const int M=256, N=256;
  cv::Mat I(M,N,CV_64F);
  cv::Mat phase(M,N,CV_64F);
  cv::Mat fx(cv::Mat::zeros(M,N,CV_64F)), fy(cv::Mat::zeros(M,N,CV_64F)),
      ffx, ffy; // las f's son las frecuencias teoricas y las
                // ff's las estimadas
  cv::Mat noise(M,N,CV_64F);
  cv::Mat fr(cv::Mat::zeros(M,N,CV_64F)), fi(cv::Mat::zeros(M,N,CV_64F)), visited;
  cv::Mat path(M,N,CV_64F);
  cv::Mat magn;
  cv::Mat hxr, hxi, hyr, hyi;
  cv::Mat h;
  cv::Mat fase;
  cv::Mat tmp;

  if(argc==1){
    // Genera datos de entrada
    //parabola(phase, 0.002);
    phase = peaks(M, N)*23;
    //phase=ramp(wx, wy, M, N);
    phase.convertTo(phase, CV_64F);
    I=cos<double>(phase);

    gradient(phase, fx, fy);
    cv::randn(noise, 0, .7);
    I=I+noise;
  }
  else{
    I=cv::imread(argv[1], 0);
    if (I.data != NULL){
      I.convertTo(I, CV_64F);
      cv::normalize(I,tmp,1,0,cv::NORM_MINMAX);
      cv::GaussianBlur(tmp, I, cv::Size(0,0), 2.7);
      cv::GaussianBlur(I, tmp, cv::Size(0,0), 17);
      I = I - tmp;
    }
    else{
      std::cerr<< "The file can not be read" << std::endl;
      return 1;
    }
  }


  cv::Vec2d freqs;
  cv::Point p;
  freqs[0]=0.7; freqs[1]=0.7;
  p.x=I.rows/2; p.y=I.cols/2;

  if(argc==1)
    std::cout<<"Frecuencia teorica local en el punto: ("<<fx.at<double>(p.y,p.x)
             <<", "<<fy.at<double>(p.y,p.x) <<")"<<std::endl;

  int i=p.y, j=p.x, cont=0;

  DemodGabor gabor(I);
  gabor.setIters(1).setKernelSize(22).
        setMaxfq(M_PI/4).setMinfq(0.01).setTau(0.3).setSeedIters(5).
        setScanMinf(.01);
  gabor.setCombFreqs(false).setCombSize(5);
  gabor.setStartPixel(p).setFreqSeed(freqs[0], freqs[1]);
  ffx = gabor.getWx();
  ffy = gabor.getWy();
  fr = gabor.getFr();
  fi = gabor.getFi();
  Scanner scan(ffx, ffy, p);
  scan.setFreqMin(.1);
  scan.updateFreqMin(false);
  cv::Point pixel;

  //gabor.run();

  do{
    // Codigo para mostrar resultados en tiempo real
    if((cont++)%500==0){
      // Genera kerneles del filtro de gabor
      pixel=scan.getPosition();
      const int i=pixel.y;
      const int j=pixel.x;
      wx = ffx.at<double>(i,j);
      wy = ffy.at<double>(i,j);
      const double w = sqrt(wx*wx + wy*wy);
      double sx = fabs(w)>0.001? fabs(M_PI_2/w):1570,
              sy = fabs(w)>0.001? fabs(M_PI_2/w):1570;
      int kernelN = gabor.getKernelSize();
      if(sx*6 > kernelN)
        sx = kernelN/6.0;
      if(sy*6 > kernelN)
        sy = kernelN/6.0;

      gen_gaborKernel(hxr, hxi, wx, sx, CV_64F);
      gen_gaborKernel(hyr, hyi, wy, sy, CV_64F);
      // Genera la parte imaginaria del filtro de gabor para desplegarlo
      h=cv::Mat::zeros(hyr.cols,hxr.cols, CV_64F)-1;
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
