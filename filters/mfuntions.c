/*
** mfuntions.c
** 
** Made by (Julio Cesar Estrada Rico)
** Login   <julio@localhost>
** 
** Started on  Tue Aug 22 21:43:10 2006 Julio Cesar Estrada Rico
** Last update Thu Sep 14 09:56:11 2006 Julio Cesar Estrada Rico
*/

/**
 *@file mfuntions.c
 *Implementacion de operadores para evaluar funciones matematicas.
 */

#include "mfuntions.h"
#include "aux.h"
#include <math.h>
#include <stdlib.h>

void math_sexp(float *restrict f, float *restrict g, const int N)
{
  register int i;
  for(i=0; i<N; i++)
    f[i]=exp(g[i]);
}

void math_dexp(double *restrict f, double *restrict g, const int N)
{
  register int i;
  for(i=0; i<N; i++)
    f[i]=exp(g[i]);
}

void math_local_datan2(double* restrict p, double* restrict s,
		       double* restrict y, double* restrict x, 
		       const int ir, const int ic,
		       const int N, const int nr, const int nc)
{
  register int i,j,k;
  const int n=N/2;
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;

  for(i=lir; i<lsr; i++)
    for(j=lic; j<lsc; j++){
      k=IDX(j,i,nr);
      p[k]=atan2(y[k],x[k]);
      if(s!=NULL)
        s[k]=1;
    }
}

void math_local_dcos(double* restrict I, double *restrict p, const int ir,
		    const int ic, const int N, const int nr, const int nc)
{
  register int i,j,k;
  const int n=N/2;
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;

  for(i=lir; i<lsr; i++)
    for(j=lic; j<lsc; j++){
      k=IDX(j,i,nr);
      I[k]=cos(p[k]);
    }
}

void math_sub_dcopy(double* restrict s, double* restrict sc, 
		    double* restrict mask, const int ir,
		    const int ic, const int nr, const int nc, const int N)
{
  register int i,j,k;
  const int n=N/2;
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;

  for(i=lir; i<lsr; i++)
    for(j=lic; j<lsc; j++){
      k=IDX(j,i,nr);
      if(mask[k]==1)
        sc[k]=s[k];
    }
}

double math_sub_ddot(double* restrict v1, double *restrict v2,
         double* restrict s,
		     const int ir, const int ic, const int nr, const int nc,
		     const int N)
{
  register int i,j,k;
  const int n=N/2;
  register double sum=0;
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;

  for(i=lir; i<lsr; i++)
    for(j=lic; j<lsc; j++){
      k=IDX(j,i,nr);
      /*El chequeo s[k]=0 tiene el fin de minimizar tiempos de procesamiento*/
      if(s!=NULL){
        if(s[k]==0)
          sum+=v1[k]*v2[k];
      }
      else
        sum+=v1[k]*v2[k];
    }

  return sum;
}
