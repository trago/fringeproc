/*
** demodula.c
** 
** Made by (Julio Cesar Estrada Rico)
** Login   <julio@localhost>
** 
** Started on  Tue Aug 29 16:10:54 2006 Julio Cesar Estrada Rico
** Last update Tue Sep 12 18:21:55 2006 Julio Cesar Estrada Rico
*/
/**
 *@file demodula.c
 *Definición de operaciones para la demodulación de interferogramas.
 */

#include "aux.h"
#include "demodula.h"
#include <math.h>

float demod_potential_s2ord(float *restrict cliq)
{
  float s=cliq[0] - 2*cliq[1] + cliq[2];
  return s*s;
}

double demod_potential_d2ord(double *restrict cliq)
{
  double s=cliq[0] - 2*cliq[1] + cliq[2];
  return s*s;
}

void demod_calc_sfrequencies(const float g, const float p, 
			     float *restrict freqs, const int dir)
{
  const float pm=atan2(sin(p),cos(p));
  const float ag=demod_calc_sacos(g);
  register float w1= (pm - ag)*dir;
  register float v1= (pm + ag)*dir;

  if (v1>M_PI)
    v1=v1-2*M_PI;
  else if (v1<-M_PI)
    v1=v1+2*M_PI;
  
  if (w1>M_PI)
    w1=w1-2*M_PI;
  else if (w1<-M_PI)
    w1=w1+2*M_PI;

  freqs[0]=v1;
  freqs[1]=w1;
}

void demod_calc_dfrequencies(const double g, const double p, 
			     double *restrict freqs, const int dir)
{
  const double pm=atan2(sin(p),cos(p));
  const double ag=demod_calc_dacos(g);
  register double w1= (pm - ag)*dir;
  register double v1= (pm + ag)*dir;

  if (v1>M_PI)
    v1=v1-2*M_PI;
  else if (v1<-M_PI)
    v1=v1+2*M_PI;
  
  if (w1>M_PI)
    w1=w1-2*M_PI;
  else if (w1<-M_PI)
    w1=w1+2*M_PI;

  freqs[0]=v1;
  freqs[1]=w1;
}

float demod_calc_sacos(const float g)
{
  float ag;
  if(g>1)
    ag=0;
  else if(g<-1)
    ag=M_PI;
  else
    ag=acos(g);
  return ag;
}

double demod_calc_dacos(const double g)
{
  double ag;
  if(g>1)
    ag=0;
  else if(g<-1)
    ag=M_PI;
  else
    ag=acos(g);
  return ag;
}

void demod_set_sseed(float *restrict g, float *restrict p)
{
  float freqs_left[2], freqs_right[2];
  register float s1,s2,s3,s4;

  demod_calc_sfrequencies(g[0],p[1],freqs_left,-1);
  demod_calc_sfrequencies(g[2],p[1],freqs_right,1);

  s1=fabs(freqs_left[0]-freqs_right[0]);
  s2=fabs(freqs_left[0]-freqs_right[1]);
  s3=fabs(freqs_left[1]-freqs_right[0]);
  s4=fabs(freqs_left[1]-freqs_right[1]);

  if(s1<=s2 && s1<=s3 && s1<=s4){
    p[2]=p[1] - freqs_right[0];
    p[0]=p[1] + freqs_left[0];
  }else if(s2<=s1 && s2<=s3 && s2<=s4){
    p[2]=p[1] - freqs_right[1];
    p[0]=p[1] + freqs_left[0];
  }else if(s3<=s1 && s3<=s2 && s3<=s4){
    p[2]=p[1] - freqs_right[0];
    p[0]=p[1] + freqs_left[1];
  }else if(s4<=s1 && s4<=s3 && s4<=s2){
    p[2]=p[1] - freqs_right[1];
    p[0]=p[1] + freqs_left[1];;
  }
}

void demod_set_dseed(double *restrict g, double *restrict p)
{
  double freqs_left[2], freqs_right[2];
  register double s1,s2,s3,s4;

  demod_calc_dfrequencies(g[0],p[1],freqs_left,-1);
  demod_calc_dfrequencies(g[2],p[1],freqs_right,1);

  s1=fabs(freqs_left[0]-freqs_right[0]);
  s2=fabs(freqs_left[0]-freqs_right[1]);
  s3=fabs(freqs_left[1]-freqs_right[0]);
  s4=fabs(freqs_left[1]-freqs_right[1]);

  if(s1<=s2 && s1<=s3 && s1<=s4){
    p[2]=p[1] - freqs_right[0];
    p[0]=p[1] + freqs_left[0];
  }else if(s2<=s1 && s2<=s3 && s2<=s4){
    p[2]=p[1] - freqs_right[1];
    p[0]=p[1] + freqs_left[0];
  }else if(s3<=s1 && s3<=s2 && s3<=s4){
    p[2]=p[1] - freqs_right[0];
    p[0]=p[1] + freqs_left[1];
  }else if(s4<=s1 && s4<=s3 && s4<=s2){
    p[2]=p[1] - freqs_right[1];
    p[0]=p[1] + freqs_left[1];;
  }
}

void demod_demodula_srow(float* restrict p, float* restrict g, 
			 const int row, const int seed, const int N, 
			 const int nr)
{
  register int i;
  register float s1,s2;
  float cliq[3];
  float freqs[2];

  for(i=seed+1; i<N-1; i++){
    demod_calc_sfrequencies(g[row + (i+1)*nr],p[row + i*nr],freqs,1);
    cliq[0]=p[row + (i-2)*nr] - p[row + (i-1)*nr];
    cliq[1]=p[row + (i-1)*nr] - p[row + i*nr];
    cliq[2]=freqs[0];
    s1=demod_potential_s2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_s2ord(cliq);

    p[row + (i+1)*nr]=(s1<s2)? (p[row + i*nr] - freqs[0]):
      (p[row + i*nr] - freqs[1]);
  }

  for(i=seed-1; i>0; i--){
    demod_calc_sfrequencies(g[row + (i-1)*nr],p[row + i*nr],freqs,-1);
    cliq[0]=p[row + (i+1)*nr] - p[row + (i+2)*nr];
    cliq[1]=p[row + i*nr] - p[row + (i+1)*nr];
    cliq[2]=freqs[0];
    s1=demod_potential_s2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_s2ord(cliq);

    p[row + (i-1)*nr]=(s1<s2)? (p[row + i*nr] + freqs[0]):
      (p[row + i*nr] + freqs[1]);
  }
}

void demod_demodula_drow(double* restrict p, double* restrict g, 
			 const int row, const int seed, const int N, 
			 const int nr)
{
  register int i;
  register double s1,s2;
  double cliq[3];
  double freqs[2];

  for(i=seed+1; i<N-1; i++){
    demod_calc_dfrequencies(g[row + (i+1)*nr],p[row + i*nr],freqs,1);
    cliq[0]=p[row + (i-2)*nr] - p[row + (i-1)*nr];
    cliq[1]=p[row + (i-1)*nr] - p[row + i*nr];
    cliq[2]=freqs[0];
    s1=demod_potential_d2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_d2ord(cliq);

    p[row + (i+1)*nr]=(s1<s2)? (p[row + i*nr] - freqs[0]):
      (p[row + i*nr] - freqs[1]);
  }

  for(i=seed-1; i>0; i--){
    demod_calc_dfrequencies(g[row + (i-1)*nr],p[row + i*nr],freqs,-1);
    cliq[0]=p[row + (i+1)*nr] - p[row + (i+2)*nr];
    cliq[1]=p[row + i*nr] - p[row + (i+1)*nr];
    cliq[2]=freqs[0];
    s1=demod_potential_d2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_d2ord(cliq);

    p[row + (i-1)*nr]=(s1<s2)? (p[row + i*nr] + freqs[0]):
      (p[row + i*nr] + freqs[1]);
  }
}

void demod_demodula_dsubrow(double* restrict p, double* restrict g, 
			    double *restrict s,
			    const int row, const int seed,
			    const int li, const int ls, 
			    const int nr)
{

  register int i;
  register double s1,s2;
  double cliq[3];
  double freqs[2];

  for(i=seed+1; i<ls-1; i++){
    //if(s[row + (i+1)*nr]==0){
      demod_calc_dfrequencies(g[row + (i+1)*nr],p[row + i*nr],freqs,1);
      cliq[0]=p[row + (i-2)*nr] - p[row + (i-1)*nr];
      cliq[1]=p[row + (i-1)*nr] - p[row + i*nr];
      cliq[2]=freqs[0];
      s1=demod_potential_d2ord(cliq);
      cliq[2]=freqs[1];
      s2=demod_potential_d2ord(cliq);
      
      p[row + (i+1)*nr]=(s1<s2)? (p[row + i*nr] - freqs[0]):
                        (p[row + i*nr] - freqs[1]);
      s[row + (i+1)*nr]=1;
    //}
  }

  for(i=seed-1; i>li; i--){
    //if(s[row + (i-1)*nr]==0){
      demod_calc_dfrequencies(g[row + (i-1)*nr],p[row + i*nr],freqs,-1);
      cliq[0]=p[row + (i+1)*nr] - p[row + (i+2)*nr];
      cliq[1]=p[row + i*nr] - p[row + (i+1)*nr];
      cliq[2]=freqs[0];
      s1=demod_potential_d2ord(cliq);
      cliq[2]=freqs[1];
      s2=demod_potential_d2ord(cliq);
      
      p[row + (i-1)*nr]=(s1<s2)? (p[row + i*nr] + freqs[0]):
                        (p[row + i*nr] + freqs[1]);
      s[row + (i-1)*nr]=1;
    //}
  }
}

void demod_demodula_scolumn(float* restrict p, float* restrict g, 
			const int column, const int seed, const int N)
{
  register int i;
  register float s1,s2;
  float cliq[3];
  float freqs[2];

  const int col=column*N;

  for(i=seed+1; i<N-1; i++){
    demod_calc_sfrequencies(g[col + i+1],p[col + i],freqs,1);
    cliq[0]=p[col + i-2] - p[col + i-1];
    cliq[1]=p[col + i-1] - p[col + i];
    cliq[2]=freqs[0];
    s1=demod_potential_s2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_s2ord(cliq);

    p[col + i+1]=(s1<s2)? (p[col + i] - freqs[0]):(p[col + i] - freqs[1]);
  }

  for(i=seed-1; i>0; i--){
    demod_calc_sfrequencies(g[col + i-1],p[col + i],freqs,-1);
    cliq[0]=p[col + i+1] - p[col + i+2];
    cliq[1]=p[col + i] - p[col + i+1];
    cliq[2]=freqs[0];
    s1=demod_potential_s2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_s2ord(cliq);

    p[col + i-1]=(s1<s2)? (p[col + i] + freqs[0]):(p[col + i] + freqs[1]);
  }
}

void demod_demodula_dcolumn(double* restrict p, double* restrict g, 
			    const int column, const int seed, const int N)
{
  register int i;
  register double s1,s2;
  double cliq[3];
  double freqs[2];

  const int col=column*N;

  for(i=seed+1; i<N-1; i++){
    demod_calc_dfrequencies(g[col + i+1],p[col + i],freqs,1);
    cliq[0]=p[col + i-2] - p[col + i-1];
    cliq[1]=p[col + i-1] - p[col + i];
    cliq[2]=freqs[0];
    s1=demod_potential_d2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_d2ord(cliq);

    p[col + i+1]=(s1<s2)? (p[col + i] - freqs[0]):(p[col + i] - freqs[1]);
  }

  for(i=seed-1; i>0; i--){
    demod_calc_dfrequencies(g[col + i-1],p[col + i],freqs,-1);
    cliq[0]=p[col + i+1] - p[col + i+2];
    cliq[1]=p[col + i] - p[col + i+1];
    cliq[2]=freqs[0];
    s1=demod_potential_d2ord(cliq);
    cliq[2]=freqs[1];
    s2=demod_potential_d2ord(cliq);

    p[col + i-1]=(s1<s2)? (p[col + i] + freqs[0]):(p[col + i] + freqs[1]);
  }
}

void demod_demodula_dsubcolumn(double* restrict p, double* restrict g, 
			       double *restrict s,
			       const int column, const int seed, const int li,
			       const int ls, const int nr)
{
  register int i;
  register double s1,s2;
  double cliq[3];
  double freqs[2];

  const int col=column*nr;

  for(i=seed+1; i<ls-1; i++){
    //if(s[col + i+1]==0){
      demod_calc_dfrequencies(g[col + i+1],p[col + i],freqs,1);
      cliq[0]=p[col + i-2] - p[col + i-1];
      cliq[1]=p[col + i-1] - p[col + i];
      cliq[2]=freqs[0];
      s1=demod_potential_d2ord(cliq);
      cliq[2]=freqs[1];
      s2=demod_potential_d2ord(cliq);
      
      p[col + i+1]=(s1<s2)? (p[col + i] - freqs[0]):(p[col + i] - freqs[1]);
      s[col + i+1]=1;
    //}
  }

  for(i=seed-1; i>li; i--){
    //if(s[col + i-1]==0){
      demod_calc_dfrequencies(g[col + i-1],p[col + i],freqs,-1);
      cliq[0]=p[col + i+1] - p[col + i+2];
      cliq[1]=p[col + i] - p[col + i+1];
      cliq[2]=freqs[0];
      s1=demod_potential_d2ord(cliq);
      cliq[2]=freqs[1];
      s2=demod_potential_d2ord(cliq);

      p[col + i-1]=(s1<s2)? (p[col + i] + freqs[0]):(p[col + i] + freqs[1]);
      s[col + i-1]=1;
    //}
  }
}

void demod_local_calc_dfreqx(double* restrict p, double* restrict s,
                             double* restrict fi, double* restrict psi,
                             double* wx, double* wy,
                             const int ir, const int ic,
                             const int nr, const int nc, const int N)
{
  const int n=N/2;
  const int k=IDX(ic,ir,nr);
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;
/*  double dr=fi[k], di=psi[k];
  
  if(ic-1>=lic){
    if(s[k-nr]==1){
      dr=fi[k]-fi[k-nr];
      di=psi[k]-psi[k-nr];
    }
  }
  else if(ic+1<lsc)
    if(s[k+nr]==1){
      dr=fi[k+nr]-fi[k];
      di=psi[k+nr]-psi[k];
    }
  *wx=-(psi[k]*dr - fi[k]*di)/(fi[k]*fi[k] + psi[k]*psi[k]);
  
  if(ir-1>=lir){
    if(s[k-1]==1){
      dr=fi[k]-fi[k-1];
      di=psi[k]-psi[k-1];
    }
  }
  else if(ir-1<lsr)
    if(s[k+1]==1){
      dr=fi[k+1]-fi[k];
      di=psi[k+1]-psi[k];
    }
  *wy=-(psi[k]*dr - fi[k]*di)/(fi[k]*fi[k] + psi[k]*psi[k]);
*/
  if(ic-1>=lic){
    if(s[k-nr]==1)
      *wx=p[k]-p[k-nr];
    else if(ic+1<lsc)
      if(s[k+nr]==1)
        *wx=p[k+nr] - p[k];
  }
  else if(ic+1<lsc){
    if(s[k+nr]==1)
      *wx=p[k+nr] - p[k];
  }

  if(ir-1>=lir){
    if(s[k-1]==1)
      *wy=p[k]-p[k-1];
    else if(ir+1<lsr)
      if(s[k+1]==1)
        *wy=p[k+1] - p[k];
  }
  else if(ir+1<lsr){
    if(s[k+1]==1)
      *wy=p[k+1] - p[k];
  }

  *wx=atan2(sin(*wx),cos(*wx));
  *wy=atan2(sin(*wy),cos(*wy));
}
