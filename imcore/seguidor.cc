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
  cv::Mat dx(nr,nc, CV_F32C1);
  cv::Mat dy(nr,nc, CV_F32C1);
  cv::Mat g=_I;

  cv::Mat qmap=_qmap;
  cv::Mat path(nr,nc, CV_F32C1);

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
   cv::Mat qmap= _qmap;
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
   _punto=Punto(i_init,j_init);
   _puntos.push_back(_punto);
   _colas[qmap.at<uchar>(i_init,j_init)].push_back(_punto);
   _caminado.at<bool>(i_init,j_init)=true;
}

inline
int Seguidor::pesoVecinos(const int i, const int j)
{
  const int nr=_I.rows;
  const int nc=_I.cols;
  const cv::Mat& caminado=_caminado;
  int peso=0;

  if(i-1>=0)
    if(caminado.at<bool>(i-1,j)==1)
      peso++;
  if(i-1>=0 && j-1>=0)
    if(caminado.at<bool>(i-1,j-1)==1)
      peso++;
  if(j-1>=0)
    if(caminado.at<bool>(i,j-1)==1)
      peso++;
  if(i+1<nc && j-1>=0)
    if(caminado.at<bool>(i+1,j-1)==1)
      peso++;
  if(i+1<nc)
    if(caminado.at<bool>(i+1,j)==1)
      peso++;
  if(i+1<nc && j+1<nr)
    if(caminado.at<bool>(i+1,j+1)==1)
      peso++;
  if(j+1<nr)
    if(caminado.at<bool>(i,j+1)==1)
      peso++;
  if(i-1>=0 && j+1<nr)
    if(caminado.at<bool>(i-1,j+1)==1)
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
  const cv::Mat &caminado=_caminado;
  Punto ptmp;
  int pesos[8];

  for(int k=0; k<8; k++){
    pesos[k]=-1;
  }

  if(i-1>=0)
    if(caminado.at<bool>(i-1,j)==0){
      ptmp=Punto(i-1,j);
      pesos[0]=pesoVecinos(i-1,j);
      vecinos[0]=ptmp;
    }
  if(i-1>=0 && j-1>=0)
    if(caminado.at<bool>(i-1,j-1)==0){
      ptmp=Punto(i-1,j-1);
      pesos[1]=pesoVecinos(i-1,j-1);
      vecinos[1]=ptmp;
    }
  if(j-1>=0)
    if(caminado.at<bool>(i,j-1)==0){
      ptmp=Punto(i,j-1);
      pesos[2]=pesoVecinos(i,j-1);
      vecinos[2]=ptmp;
    }
  if(i+1<nc && j-1>=0)
    if(caminado.at<bool>(i+1,j-1)==0){
      ptmp=Punto(i+1,j-1);
      pesos[3]=pesoVecinos(i+1,j-1);
      vecinos[3]=ptmp;
    }
  if(i+1<nc)
    if(caminado.at<bool>(i+1,j)==0){
      ptmp=Punto(i+1,j);
      pesos[4]=pesoVecinos(i+1,j);
      vecinos[4]=ptmp;
    }
  if(i+1<nc && j+1<nr)
    if(caminado.at<bool>(i+1,j+1)==0){
      ptmp=Punto(i+1,j+1);
      pesos[5]=pesoVecinos(i+1,j+1);
      vecinos[5]=ptmp;
    }
  if(j+1<nr)
    if(caminado.at<bool>(i,j+1)==0){
      ptmp=Punto(i,j+1);
      pesos[6]=pesoVecinos(i,j+1);
      vecinos[6]=ptmp;
    }
  if(i-1>=0 && j+1<nr)
    if(caminado.at<bool>(i-1,j+1)==0){
      ptmp=Punto(i-1,j+1);
      pesos[7]=pesoVecinos(i-1,j+1);
      vecinos[7]=ptmp;
    }

  int aux;
  bool ordenado=false;
  for(int k=1; k<7; k++){
    if(ordenado)
      break;
    ordenado=true;
    for(int l=0; l<7-k; l++){
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
}

void Seguidor::cargaVecinos()
{
  const cv::Mat &caminado=_caminado;
  const cv::Mat &qmap=_qmap;
  Punto ptmp;
  Punto vecinos[8];

  ordenaVecinos(vecinos);
  for(int k=0; k<8; k++)
    if(vecinos[k].valid){
      _puntos.push_back(vecinos[k]);
      _colas[qmap.at<uchar>(vecinos[k].r,vecinos[k].c)].push_back(vecinos[k]);
      caminado.at<bool>(vecinos[k].r,vecinos[k].c)=true;
    }
}

Seguidor::Seguidor(const cv::Mat& I,int levels)
{
  _I=I;
  _colas=new std::vector<Punto>[levels];
  _m.resize(_I.rows(),_I.cols());
  _caminado.resize(_I.rows(),_I.cols());
  _qmap.resize(_I.rows(),_I.cols());
  _levels=levels;
  _m=0;
  _caminado=0;
  _qmap=0;
  _punto=NULL;
  //la variable _punto se inicializa en set_inicio()
  //Primero calculamos el mapa de calidad por niveles
  calcQualityMap(levels);
  //Finalmente establecemos el punto inicial
  set_inicio();
  cargaVecinos();
}
Seguidor::Seguidor(const sArray& I,int r, int c, int levels)
{
  _I=I;
  _colas=new std::list<Punto*>[(int)levels];
  _m.resize(_I.rows(),_I.cols());
  _caminado.resize(_I.rows(),_I.cols());
  _qmap.resize(_I.rows(),_I.cols());
  _levels=levels;
  _m=0;
  _caminado=0;
  _qmap=0;
  _punto=NULL;
  //la variable _punto se inicializa en set_inicio()
  //Primero calculamos el mapa de calidad por niveles
  calcQualityMap(levels);
  //Finalmente establecemos el punto inicial
  _punto=new Punto(r,c);
  _puntos.push_back(_punto);
  _colas[(int)_qmap[r][c]].push_back(_punto);
  _caminado[r][c]=1;
  cargaVecinos();
}

int Seguidor::get_c()
{
  if(_punto!=NULL)
    return _punto->c;
  return -1;
}

int Seguidor::get_r()
{
  if(_punto!=NULL)
    return _punto->r;
  return -1;
}

sArray Seguidor::get_qmap()
{
  return _qmap;
}

bool Seguidor::siguiente()
{
  std::list<Punto*>* cola=NULL;
  for(int i=_levels-1; i>=0; i--)
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
  _punto=NULL;
  return false;
}

void Seguidor::setQMap(sArray& qmap)
{
  _qmap=qmap;
  sArray& map=_qmap;

  double a,b;
  a=map[0][0];
  b=a;
  for(int i=0; i<_qmap.rows(); i++)
    for(int j=0; j<_qmap.cols(); j++){
      a=(a>map[i][j])? map[i][j]:a;
      b=(b<map[i][j])? map[i][j]:b;
    }
  int levels=_levels;
  double step=(b-a)/levels;
  for(int i=0; i<_qmap.rows(); i++)
    for(int j=0; j<_qmap.cols(); j++){
      for(int l=0; l<levels; l++)
        if(map[i][j]>=(a+l*step) && map[i][j]<=((l+1)*step))
          map[i][j]=l;
    }
}

Seguidor::~Seguidor()
{
  std::list<Punto*>::iterator j;

  int n=0, N=_I.rows()*_I.cols();
  for(j=_puntos.begin(); j!=_puntos.end() && n<N; j++, n++){
    delete *j;
  }

  delete []_colas;
}
