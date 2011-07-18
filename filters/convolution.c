/*
** convolution.c
** 
** Made by (Julio Cesar Estrada Rico)
** Login   <julio@localhost>
** 
** Started on  Mon Aug 21 15:28:25 2006 Julio Cesar Estrada Rico
** Last update Sun Sep  3 11:57:42 2006 Julio Cesar Estrada Rico
*/

/**
 *@file convolution.c
 *Implementacion de los operadores para la convolucion.
 */


#include "convolution.h"
#include "aux.h"
#include <cblas.h>
#include <stdlib.h>

void convolution_snormalized(float *restrict f, float *restrict G, 
			      float *restrict hx, float *restrict hy,
            const int nr, const int nc, 
            const int kr, const int kc)
{
  const int rf=nr;
  const int cf=nc;
  const int N=kr,M=kc;

  int r,c;/*Indices para recorrer renglones y columnas*/
  int n; /*Valores que indican dimenciones parciales de vectores*/
  int Lif, Lih; /*L\'imites inferiores de convoluci\'on*/
  int Lsf, Lsh; /*L\'imites superiores de convoluci\'on*/
  int fi,hi; /*Indices para recorrer el area de convoluci\'on*/

  float *restrict g=malloc(nr*nc*sizeof(float));

  cblas_scopy(nr*nc,G,1,g,1);

  n=M/2;
  /*Convolvemos renglones*/
  for(r=0; r<rf; r++)
    for(c=0;c<cf; c++){
      /*Calculamos los l\'imites del \'area de convoluci\'on*/
      if(c-n>=0 && c+M-n<cf){
        Lif=c-n;
        Lsf=c-M-n;
        Lih=0;
        Lsh=M;
      }
      else if(c-n<0 && c+M-n<cf){
        Lif=0;
        Lsf=c+M-n;
        Lih=n-c;
        Lsh=M;
      }
      else if(c-n>=0 && c+M-n>=cf){
        Lif=c-n;
        Lsf=cf;
        Lih=0;
        Lsh=n+cf-c;
      }
      else{
        Lif=0;
        Lsf=cf;
        Lih=n-c;
        Lsh=n+cf-c;
      }

      float sum=0;
      g[IDX(c,r,nr)]=0;
      for(fi=Lif, hi=Lih; hi<Lsh; fi++,hi++){
        g[IDX(c,r,nr)]+=G[IDX(fi,r,nr)]*hx[hi];
        sum+=hx[hi];
      }
      if(sum!=0)
        g[IDX(c,r,nr)]/=sum;
    }
    /*Convolvemos renglones*/
    n=N/2;
    for(c=0; c<cf; c++)
      for(r=0;r<rf; r++){
        /*Calculamos los l\'imites del \'area de convoluci\'on*/
        if(r-n>=0 && r+N-n<rf){
          Lif=r-n;
          Lsf=r-N-n;
          Lih=0;
          Lsh=N;
        }
        else if(r-n<0 && r+N-n<rf){
          Lif=0;
          Lsf=r+N-n;
          Lih=n-r;
          Lsh=N;
        }
        else if(r-n>=0 && r+N-n>=rf){
          Lif=r-n;
          Lsf=rf;
          Lih=0;
          Lsh=n+rf-r;
        }
        else{
          Lif=0;
          Lsf=rf;
          Lih=n-r;
          Lsh=n+rf-r;
        }

        float sum=0;
        f[IDX(c,r,nr)]=0;
        for(fi=Lif, hi=Lih; hi<Lsh; fi++,hi++){
          f[IDX(c,r,nr)]+=g[IDX(c,fi,nr)]*hy[hi];
          sum+=hy[hi];
        }
        if(sum!=0)
          f[IDX(c,r,nr)]/=sum;
    }

  free(g);
}

void convolution_dnormalized(double *restrict f, double *restrict G, 
			     double *restrict hx, double *restrict hy,
			     const int nr, const int nc, 
			     const int kr, const int kc)
{
  const int rf=nr;
  const int cf=nc;
  const int N=kr,M=kc;

  int r,c;/*Indices para recorrer renglones y columnas*/
  int n; /*Valores que indican dimenciones parciales de vectores*/
  int Lif, Lih; /*L\'imites inferiores de convoluci\'on*/
  int Lsf, Lsh; /*L\'imites superiores de convoluci\'on*/
  int fi,hi; /*Indices para recorrer el area de convoluci\'on*/

  double *restrict g=malloc(nr*nc*sizeof(double));

  cblas_dcopy(nr*nc,G,1,g,1);

  n=M/2;
  /*Convolvemos renglones*/
  for(r=0; r<rf; r++)
    for(c=0;c<cf; c++){
      /*Calculamos los l\'imites del \'area de convoluci\'on*/
      if(c-n>=0 && c+M-n<cf){
        Lif=c-n;
        Lsf=c-M-n;
        Lih=0;
        Lsh=M;
      }
      else if(c-n<0 && c+M-n<cf){
        Lif=0;
        Lsf=c+M-n;
        Lih=n-c;
        Lsh=M;
      }
      else if(c-n>=0 && c+M-n>=cf){
        Lif=c-n;
        Lsf=cf;
        Lih=0;
        Lsh=n+cf-c;
      }
      else{
        Lif=0;
        Lsf=cf;
        Lih=n-c;
        Lsh=n+cf-c;
      }

      double sum=0;
      g[IDX(c,r,nr)]=0;
      for(fi=Lif, hi=Lih; hi<Lsh; fi++,hi++){
        g[IDX(c,r,nr)]+=G[IDX(fi,r,nr)]*hx[hi];
        sum+=hx[hi];
      }
      if(sum!=0)
        g[IDX(c,r,nr)]/=sum;
    }
    /*Convolvemos renglones*/
    n=N/2;
    for(c=0; c<cf; c++)
      for(r=0;r<rf; r++){
        /*Calculamos los l\'imites del \'area de convoluci\'on*/
        if(r-n>=0 && r+N-n<rf){
          Lif=r-n;
          Lsf=r-N-n;
          Lih=0;
          Lsh=N;
        }
        else if(r-n<0 && r+N-n<rf){
          Lif=0;
          Lsf=r+N-n;
          Lih=n-r;
          Lsh=N;
        }
        else if(r-n>=0 && r+N-n>=rf){
          Lif=r-n;
          Lsf=rf;
          Lih=0;
          Lsh=n+rf-r;
        }
        else{
          Lif=0;
          Lsf=rf;
          Lih=n-r;
          Lsh=n+rf-r;
        }

        double sum=0;
        f[IDX(c,r,nr)]=0;
        for(fi=Lif, hi=Lih; hi<Lsh; fi++,hi++){
          f[IDX(c,r,nr)]+=g[IDX(c,fi,nr)]*hy[hi];
          sum+=hy[hi];
        }
        if(sum!=0)
          f[IDX(c,r,nr)]/=sum;
    }

    free(g);
}
