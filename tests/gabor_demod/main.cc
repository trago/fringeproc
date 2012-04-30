#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <imcore/seguidor.h>
#include <utils/utils.h>
#include <iostream>
#include "gabor_gears.h"

void gradient(const cv::Mat I, cv::Mat& dx, cv::Mat& dy)
{
  dx.create(I.rows, I.cols, I.type());
  dy.create(I.rows, I.cols, I.type());

  for(int i=0; i<I.rows-1; i++)
    for(int j=0; j<I.cols-1; j++){
      dx.at<float>(i,j) = I.at<float>(i,j+1)-I.at<float>(i,j);
      dy.at<float>(i,j) = I.at<float>(i+1,j)-I.at<float>(i,j);
    }
  for(int i=0; i<I.rows; i++)
    dy.at<float>(i,I.cols-1)=dy.at<float>(i,I.cols-2);
  for(int i=0; i<I.cols; i++)
    dx.at<float>(I.rows-1,i)=dx.at<float>(I.rows-2,i);
}

inline
void filtraNeighborhood(const cv::Mat I, cv::Mat fr, cv::Mat fi,
                        float wx, float wy, int i, int j)
{
  if(i-1>=0)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j, i-1);
  else if(i+1<I.rows)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j, i+1);
  if(j-1>=0)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j-1, i);
  else if(j+1<I.cols)
   gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                          j+1, i);
  gabor_adaptiveFilterXY(I, fr, fi, wx, wy,
                         j, i);
}
inline
void filtraNeighborhood(const cv::Mat I, cv::Mat fr, cv::Mat fi,
                        cv::Mat fx, cv::Mat fy, cv::Mat visited,
                        float wx, float wy, int i, int j, const int N)
{
  int liM = (i-N/2>=0)? i-N/2:0, lsM = (i+N/2<I.rows)? i+N/2:I.rows-1;
  int liN = (j-N/2>=0)? j-N/2:0, lsN = (j+N/2<I.cols)? j+N/2:I.cols-1;
  cv::Vec2d freqs;
  for(int m=i; m<lsM; m++){
    freqs = peak_freqXY(fx, fy, visited, j, m);
    wx=freqs[0];
    wy=freqs[1];
    for(int n=j; n<lsN; n++){
      if(visited.at<char>(m,n)==0){
        filtraNeighborhood(I, fr, fi, wx, wy, m, n);
        freqs=calc_freqXY(fr, fi, n, m);
        fx.at<float>(m,n)=freqs[0];
        fy.at<float>(m,n)=freqs[1];
        wx=fx.at<float>(m,n);
        wy=fy.at<float>(m,n);
        visited.at<char>(m,n)=1;
      }
    }
    freqs = peak_freqXY(fx, fy, visited, j-1, m);
    wx=freqs[0];
    wy=freqs[1];
    for(int n=j-1; n>=liN; n--){
      if(visited.at<char>(m,n)==0){
        filtraNeighborhood(I, fr, fi, wx, wy, m, n);
        freqs=calc_freqXY(fr, fi, n, m);
        fx.at<float>(m,n)=freqs[0];
        fy.at<float>(m,n)=freqs[1];
        wx=fx.at<float>(m,n);
        wy=fy.at<float>(m,n);
        visited.at<char>(m,n)=1;
      }
    }
  }
  for(int m=i-1; m>=liM; m--){
    freqs = peak_freqXY(fx, fy, visited, j, m);
    wx=freqs[0];
    wy=freqs[1];
    for(int n=j; n<lsN; n++){
      if(visited.at<char>(m,n)==0){
        filtraNeighborhood(I, fr, fi, wx, wy, m, n);
        freqs=calc_freqXY(fr, fi, n, m);
        wx=fx.at<float>(m,n)=freqs[0];
        fy.at<float>(m,n)=freqs[1];
        fx.at<float>(m,n);
        wy=fy.at<float>(m,n);
        visited.at<char>(m,n)=1;
      }
    }
    freqs = peak_freqXY(fx, fy, visited, j-1, m);
    wx=freqs[0];
    wy=freqs[1];
    for(int n=j-1; n>=liN; n--){
      if(visited.at<char>(m,n)==0){
        filtraNeighborhood(I, fr, fi, wx, wy, m, n);
        freqs=calc_freqXY(fr, fi, n, m);
        fx.at<float>(m,n)=freqs[0];
        fy.at<float>(m,n)=freqs[1];
        wx=fx.at<float>(m,n);
        wy=fy.at<float>(m,n);
        visited.at<char>(m,n)=1;
      }
    }
  }

}

int main()
{
  float wx= 1.1, wy=0.;
  const int M=456, N=456;
  cv::Mat I(M,N,CV_32F);
  cv::Mat phase(M,N,CV_32F);
  cv::Mat fx(cv::Mat::zeros(M,N,CV_32F)), fy(cv::Mat::zeros(M,N,CV_32F)),
      ffx, ffy; // las f's son las frecuencias teoricas y las ff's las estimadas
  cv::Mat noise(M,N,CV_32F);
  cv::Mat fr(cv::Mat::zeros(M,N,CV_32F)), fi(cv::Mat::zeros(M,N,CV_32F)),
      visited;
  cv::Mat path(M,N,CV_32F);
  cv::Mat magn;
  cv::Mat hxr, hxi, hyr, hyi;
  cv::Mat h;
  cv::Mat fase;
  cv::Mat tmp;

  // Genera datos de entrada
  parabola(phase, 0.0005);
  phase += peaks(M, N)*33;
  //phase=ramp(wx, wy, M, N);
  I=cos<float>(phase);
  gradient(phase, fx, fy);
  cv::randn(noise, 0, 0.001);
  I=I+noise;


  // Aplica filtro y busca el pixel que esta entonado con el filtro.
  // despues calcula su frecuencia local
  gabor_filter(I, fr, fi, wx, wy);
  cv::magnitude(fr, fi, magn);
  double min, max;
  cv::Point2i p;
  cv::minMaxLoc(magn, &min, &max, NULL, &p);
  std::cout<<"Puno inicial: ("<<p.x<<", "<<p.y<<")"<<std::endl;
  //I.at<float>(p.y, p.x)=20;
  cv::Vec2d freqs = calc_freqXY(fr, fi, p.x, p.y);
  std::cout<<"Frecuencia local en el punto: ("<<freqs[0]<<", "<<freqs[1]
          <<")"<<std::endl;
  std::cout<<"Frecuencia teorica local en el punto: ("<<fx.at<float>(p.y,p.x)
           <<", "<<fy.at<float>(p.y,p.x) <<")"<<std::endl;


  // Demodula siguiendo franjas usando filtro de gabor adaptivo
  // tomando las frecuancias teoricas para entonar los filtros de gabor

  cv::GaussianBlur(I, path, cv::Size(5,5),0);
  Seguidor scan(I, p.y, p.x, 9);
  //scan.siguiente(); // Parece que el primer punto lo repite
  int i=p.y, j=p.x, cont=0;
  ffx = cv::Mat::zeros(I.rows, I.cols, CV_32F);
  ffy = cv::Mat::zeros(I.rows, I.cols, CV_32F);
  fr = cv::Mat::zeros(I.rows, I.cols, CV_32F);
  fi = cv::Mat::zeros(I.rows, I.cols, CV_32F);
  ffx.at<float>(i,j)=freqs[0];
  ffy.at<float>(i,j)=freqs[1];
  visited = cv::Mat::zeros(I.rows, I.cols, CV_8U);

  do{
    i=scan.get_r();
    j=scan.get_c();
    if((i==p.y && j==p.x)){
      filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
      freqs = calc_freqXY(fr, fi, j, i);
      ffx.at<float>(i,j)=freqs[0];
      ffy.at<float>(i,j)=freqs[1];
      visited.at<char>(i,j)=1;
    }
    else{
      freqs = peak_freqXY(ffx, ffy, visited, j, i);

      filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
      //filtraNeighborhood(I,fr,fi,ffx,ffy,visited,freqs[0],freqs[1],i,j,150);
      //freqs = calc_freqXY(fr, fi, ffx, ffy, visited, j, i);
      freqs = calc_freqXY(fr, fi, j, i);
      ffx.at<float>(i,j)=freqs[0];
      ffy.at<float>(i,j)=freqs[1];
      visited.at<char>(i,j)=1;

    }
    if((cont++)%5000==1000){
      // Genera kerneles del filtro de gabor
      wx = ffx.at<float>(i,j);
      wy = ffy.at<float>(i,j);
      float sx = fabs(1.5708/wx), sy = fabs(1.5708/wy);
      sx = sx>22? 22:(sx<1? 1:sx);
      sy = sy>22? 22:(sy<1? 1:sy);
      gen_gaborKernel(hxr, hxi, wx, sx, CV_32F);
      gen_gaborKernel(hyr, hyi, wy, sy, CV_32F);
      // Genera la parte imaginaria del filtro de gabor para desplegarlo
      h=cv::Mat::zeros(156,156, CV_32F)-1;
      for(int i=0; i<hyr.cols; i++)
        for(int j=0; j<hxr.cols; j++)
          h.at<float>(i,j)=hxr.at<float>(0,j)*hyi.at<float>(0,i) +
              hxi.at<float>(0,j)*hyr.at<float>(0,i);

      cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
      cv::imshow("real", tmp);
      cv::normalize(h,tmp,1,0,cv::NORM_MINMAX);
      cv::imshow("kernel", tmp);
      cv::waitKey(32);
    }
  }while(scan.siguiente());

  /*
  for(int i=0; i<I.rows; i++)
   for(int j=0; j<I.cols; j++){
     if(!(i==0 && j==0))
      freqs = peak_freqXY(ffx, ffy, visited, j, i);

    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    filtraNeighborhood (I, fr, fi, freqs[0], freqs[1], i,j);
    freqs = calc_freqXY(fr, fi, j, i);
    //freqs = calc_freqXY(fr, fi, ffx, ffy, visited, j, i);

     ffx.at<float>(i,j)=freqs[0];
     ffy.at<float>(i,j)=freqs[1];
     visited.at<char>(i,j)=1;
     if((cont++)%5000==1000){
       // Genera kerneles del filtro de gabor
       wx = ffx.at<float>(i,j);
       wy = ffy.at<float>(i,j);
       float sx = fabs(1.5708/wx), sy = fabs(1.5708/wy);
       sx = sx>22? 22:(sx<1? 1:sx);
       sy = sy>22? 22:(sy<1? 1:sy);
       gen_gaborKernel(hxr, hxi, wx, sx, CV_32F);
       gen_gaborKernel(hyr, hyi, wy, sy, CV_32F);
       // Genera la parte imaginaria del filtro de gabor para desplegarlo
       h=cv::Mat::zeros(156,156, CV_32F)-1;
       for(int i=0; i<hyr.cols; i++)
        for(int j=0; j<hxr.cols; j++)
         h.at<float>(i,j)=hxr.at<float>(0,j)*hyi.at<float>(0,i) +
         hxi.at<float>(0,j)*hyr.at<float>(0,i);

       cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
       cv::imshow("real", tmp);
       cv::normalize(h,tmp,1,0,cv::NORM_MINMAX);
       cv::imshow("kernel", tmp);
       cv::waitKey(32);
      }
    }
   */
  // Calculo de la fase de salida
  fase = atan2<float>(fi,fr);
  cv::magnitude(fr, fi, magn);

  cv::namedWindow("I");
  cv::namedWindow("real");
  cv::namedWindow("imag");
  cv::namedWindow("fx");
  cv::namedWindow("fx");
  cv::namedWindow("ffy");
  cv::namedWindow("ffy");
  cv::namedWindow("kernel");
  cv::namedWindow("fase");

  cv::normalize(I,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("I", tmp);
  cv::normalize(fr,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("real", tmp);
  cv::normalize(fi,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("imag", tmp);
  cv::normalize(fase,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("fase", tmp);
  cv::normalize(fx,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("fx", tmp);
  cv::normalize(fy,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("fy", tmp);
  cv::normalize(ffx,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("ffx", tmp);
  cv::normalize(ffy,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("ffy", tmp);
  cv::normalize(magn,tmp,1,0,cv::NORM_MINMAX);
  cv::imshow("kernel", tmp);

  cv::waitKey(0);

  return 0;
}
