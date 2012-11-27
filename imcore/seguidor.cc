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

#include "seguidor.h"
#include <cmath>

Punto::Punto(int row, int col)
{
  c=col;
  r=row;
  valid=true;
}
Punto::Punto()
{
  r=0;
  c=0;
  valid=false;
}

inline
void Punto::set(const int row, const int col, const bool valid)
{
  r=row;
  c=col;
  this->valid=valid;
}

Punto::Punto(const Punto& cpy)
{
  r=cpy.r;
  c=cpy.c;
  valid=cpy.valid;
}

void Seguidor::calcQualityMap(int levels)
{
  int nr=_I.rows;
  int nc=_I.cols;
  Eigen::ArrayXXf dx(nr,nc, CV_32FC1);
  Eigen::ArrayXXf dy(nr,nc, CV_32FC1);
  Eigen::ArrayXXf g=_I;

  Eigen::ArrayXXf qmap=_qmap;
  Eigen::ArrayXXf path(nr,nc, CV_32FC1);

  double a=0,b=0;
  for(int i=0; i<nr; i++)
    for(int j=0; j<nc; j++){
      if(i-1>=0)
        dx.at<float>(i,j)=g.at<float>(i,j) - g.at<float>(i-1,j);
      else
        dx.at<float>(i,j)=g.at<float>(i+1,j) - g.at<float>(i,j);
      if(j-1>=0)
        dy.at<float>(i,j)=g.at<float>(i,j) - g.at<float>(i,j-1);
      else
        dy.at<float>(i,j)=g.at<float>(i,j+1) - g.at<float>(i,j);
      path.at<float>(i,j)=dx.at<float>(i,j)*dx.at<float>(i,j) +
          dy.at<float>(i,j)*dy.at<float>(i,j);
      a=(a>path.at<float>(i,j))? path.at<float>(i,j):a;
      b=(b<path.at<float>(i,j))? path.at<float>(i,j):b;
    }
    double step=(b-a)/levels;
    for(int i=0; i<nr; i++)
      for(int j=0; j<nc; j++){
        for(int l=0; l<levels; l++)
          if(path.at<float>(i,j)>=(a+l*step) && path.at<float>(i,j)<=((l+1)*step))
            qmap.at<uchar>(i,j)=l;
      }
}

void Seguidor::set_inicio()
{
   int i_init=0, j_init=0;
   Eigen::ArrayXXf qmap= _qmap;
   int nr=_qmap.rows;
   int nc=_qmap.cols;

   //Primero se busca el punto con nivel mas alto de calidad
   uchar level=0;
   for(int i=0; i<nr; i++)
     for(int j=0; j<nc; j++){
       if(level<qmap.at<uchar>(i,j)){
         level=qmap.at<uchar>(i,j);
         i_init=i;
         j_init=j;
         break;
       }
     }
   _punto.set(i_init,j_init);
   _colas[qmap.at<uchar>(i_init,j_init)].push_back(_punto);
   _caminado.at<uchar>(i_init,j_init)=true;
}

inline
int Seguidor::pesoVecinos(const int i, const int j)
{
  const int nr=_I.rows;
  const int nc=_I.cols;
  const Eigen::ArrayXXf& caminado=_caminado;
  int peso=0;

  if(i-1>=0)
    if(caminado.at<uchar>(i-1,j)==1)
      peso++;
  if(i-1>=0 && j-1>=0)
    if(caminado.at<uchar>(i-1,j-1)==1)
      peso++;
  if(j-1>=0)
    if(caminado.at<uchar>(i,j-1)==1)
      peso++;
  if(i+1<nr && j-1>=0)
    if(caminado.at<uchar>(i+1,j-1)==1)
      peso++;
  if(i+1<nr)
    if(caminado.at<uchar>(i+1,j)==1)
      peso++;
  if(i+1<nr && j+1<nc)
    if(caminado.at<uchar>(i+1,j+1)==1)
      peso++;
  if(j+1<nc)
    if(caminado.at<uchar>(i,j+1)==1)
      peso++;
  if(i-1>=0 && j+1<nc)
    if(caminado.at<uchar>(i-1,j+1)==1)
      peso++;
  return peso;
}

inline
void Seguidor::ordenaVecinos(Punto vecinos[])
{
  int i=_punto.r;
  int j=_punto.c;
  int nr=_I.rows;
  int nc=_I.cols;
  Punto ptmp;
  int pesos[8];

  for(int k=0; k<8; k++){
    pesos[k]=-1;
  }

  if(i-1>=0)
    if(_caminado.at<uchar>(i-1,j)==0){
      ptmp.set(i-1,j);
      pesos[0]=pesoVecinos(i-1,j);
      vecinos[0]=ptmp;
      _caminado.at<uchar>(i-1,j)=1;
    }
  if(i-1>=0 && j-1>=0)
    if(_caminado.at<uchar>(i-1,j-1)==0){
      ptmp.set(i-1,j-1);
      pesos[1]=pesoVecinos(i-1,j-1);
      vecinos[1]=ptmp;
      _caminado.at<uchar>(i-1,j-1)=1;
    }
  if(j-1>=0)
    if(_caminado.at<uchar>(i,j-1)==0){
      ptmp.set(i,j-1);
      pesos[2]=pesoVecinos(i,j-1);
      vecinos[2]=ptmp;
      _caminado.at<uchar>(i,j-1)=1;
    }
  if(i+1<nr && j-1>=0)
    if(_caminado.at<uchar>(i+1,j-1)==0){
      ptmp.set(i+1,j-1);
      pesos[3]=pesoVecinos(i+1,j-1);
      vecinos[3]=ptmp;
      _caminado.at<uchar>(i+1,j-1)=1;
    }
  if(i+1<nr)
    if(_caminado.at<uchar>(i+1,j)==0){
      ptmp.set(i+1,j);
      pesos[4]=pesoVecinos(i+1,j);
      vecinos[4]=ptmp;
      _caminado.at<uchar>(i+1,j)=1;
    }
  if(i+1<nr && j+1<nc)
    if(_caminado.at<uchar>(i+1,j+1)==0){
      ptmp.set(i+1,j+1);
      pesos[5]=pesoVecinos(i+1,j+1);
      vecinos[5]=ptmp;
      _caminado.at<uchar>(i+1,j+1)=1;
    }
  if(j+1<nc)
    if(_caminado.at<uchar>(i,j+1)==0){
      ptmp.set(i,j+1);
      pesos[6]=pesoVecinos(i,j+1);
      vecinos[6]=ptmp;
      _caminado.at<uchar>(i,j+1)=1;
    }
  if(i-1>=0 && j+1<nc)
    if(_caminado.at<uchar>(i-1,j+1)==0){
      ptmp.set(i-1,j+1);
      pesos[7]=pesoVecinos(i-1,j+1);
      vecinos[7]=ptmp;
      _caminado.at<uchar>(i-1,j+1)=1;
    }

  /*
  int aux;
  bool ordenado=false;
  for(int k=1; k<8; k++){
    if(ordenado)
      break;
    ordenado=true;
    for(int l=0; l<8-k; l++){
      if(pesos[l]<pesos[l+1]){
        aux=pesos[k];
        pesos[k]=pesos[l];
        pesos[l]=aux;
        ptmp=vecinos[k];
        vecinos[k]=vecinos[l];
        vecinos[l]=ptmp;
        ordenado=false;
      }
    }
  }
   */
}

void Seguidor::cargaVecinos()
{
  Punto ptmp;
  Punto vecinos[8];

  ordenaVecinos(vecinos);
  for(int k=0; k<8; k++)
    if(vecinos[k].valid){
      _colas[_qmap.at<uchar>(vecinos[k].r,vecinos[k].c)].push_back(vecinos[k]);
    }
}

Seguidor::Seguidor(const Eigen::ArrayXXf& I,int levels)
{
  _I=I;
  _colas=new std::list<Punto>[levels];
  _m=Eigen::ArrayXXf::zeros(_I.rows, _I.cols, CV_8U);
  _caminado=Eigen::ArrayXXf::zeros(_I.rows, _I.cols, CV_8U);
  _qmap=Eigen::ArrayXXf::zeros(_I.rows, _I.cols, CV_8U);
  _levels=levels;
  //la variable _punto se inicializa en set_inicio()
  //Primero calculamos el mapa de calidad por niveles
  calcQualityMap(levels);
  //Finalmente establecemos el punto inicial
  set_inicio();
  cargaVecinos();
}
Seguidor::Seguidor(const Eigen::ArrayXXf& I,int r, int c, int levels)
{
  _I=I;
  _colas=new std::list<Punto>[(int)levels];
  _m=Eigen::ArrayXXf::zeros(_I.rows, _I.cols, CV_8U);
  _caminado=Eigen::ArrayXXf::zeros(_I.rows, _I.cols, CV_8U);
  _qmap=Eigen::ArrayXXf::zeros(_I.rows, _I.cols, CV_8U);
  _levels=levels;
  //la variable _punto se inicializa en set_inicio()
  //Primero calculamos el mapa de calidad por niveles
  calcQualityMap(levels);
  //Finalmente establecemos el punto inicial
  _punto.set(r,c);
  _colas[_qmap.at<uchar>(r,c)].push_back(_punto);
  _caminado.at<uchar>(r,c)=1;
  cargaVecinos();
}

int Seguidor::get_c()
{
  return _punto.c;
}

int Seguidor::get_r()
{
  return _punto.r;
}

Eigen::ArrayXXf Seguidor::get_qmap()
{
  return _qmap;
}

bool Seguidor::siguiente()
{
  std::list<Punto>* cola=NULL;
  int level=_qmap.at<char>(_punto.r, _punto.c);
  for(int i=level; i>=0; i--)
    if(_colas[i].begin()!=_colas[i].end()){
      cola=_colas+i;
      break;
    }
  for(int i=level+1; i<_levels; i++)
    if(_colas[i].begin()!=_colas[i].end()){
      cola=_colas+i;
      break;
    }
  if(cola!=NULL){
    _punto=cola->front();
    cola->pop_front();
    cargaVecinos();
    return true;
  }
  _punto.valid=false;
  return false;
}

void Seguidor::setQMap(const Eigen::ArrayXXf& qmap)
{
  _qmap.create(qmap.rows, qmap.cols, CV_8U);
  Eigen::ArrayXXf map=qmap;

  double a,b;
  a=map.at<uchar>(0,0);
  b=a;
  for(int i=0; i<_qmap.rows; i++)
    for(int j=0; j<_qmap.cols; j++){
      a=(a>map.at<uchar>(i,j))? map.at<uchar>(i,j):a;
      b=(b<map.at<uchar>(i,j))? map.at<uchar>(i,j):b;
    }
  int levels=_levels;
  double step=(b-a)/levels;
  for(int i=0; i<_qmap.rows; i++)
    for(int j=0; j<_qmap.cols; j++){
      for(int l=0; l<levels; l++)
        if(map.at<uchar>(i,j)>=(a+l*step) && map.at<uchar>(i,j)<=((l+1)*step))
          _qmap.at<uchar>(i,j)=l;
    }
}

Seguidor::~Seguidor()
{
  delete []_colas;
}
