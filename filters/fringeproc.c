/*
** fringeproc.c
** 
** Made by (Julio Cesar Estrada Rico)
** Login   <julio@localhost>
** 
** Started on  Mon Aug 21 15:18:03 2006 Julio Cesar Estrada Rico
** Last update Thu Oct 19 10:33:33 2006 Julio Cesar Estrada Rico
*/

/**
 *@file fringeproc.c
 *Definiciones de metodos y operaciones para el procesamiento de franjas.
 */

#include <stdlib.h>
#include <cblas.h>
#include <math.h>
#include "aux.h"
#include "fringeproc.h"
#include "mfuntions.h"
#include "convolution.h"
#include "demodula.h"
#include "optimization.h"
#include "follower.h"

int  filter_sgaussian(float *f, float *g, 
		      const float sigma, const int nr, const int nc)
{
  register int i;
  const int kr=(int)6*sigma;
  const int kc=(int)6*sigma;

  float *restrict hx=malloc(kc*sizeof(float));
  float *restrict hy=malloc(kr*sizeof(float));

  if(hx==NULL || hy==NULL)
    return 1;
  
  /*Generamos el kernel en direccion x*/
  for(i=0; i<kc; i++){
    hx[i]=-(i-kc/2)*(i-kc/2);
    hx[i]/=sigma*sigma;
  }
  math_sexp(hx,hx,kc);

  /*Generamos el kernel en direccion x*/
  for(i=0; i<kr; i++){
    hy[i]=-(i-kr/2)*(i-kr/2);
    hy[i]/=sigma*sigma;
  }
  math_sexp(hy,hy,kr);

  convolution_snormalized(f,g,hx,hy,nr,nc,kr,kc);

  free(hx);
  free(hy);

  return 0;
}

int  filter_dgaussian(double *f, double *g, 
		      const double sigma, const int nr, const int nc)
{
  register int i;
  const int kr=(int)6*sigma;
  const int kc=(int)6*sigma;

  double *restrict hx=malloc(kc*sizeof(double));
  double *restrict hy=malloc(kr*sizeof(double));

  if(hx==NULL || hy==NULL)
    return 1;
  
  /*Generamos el kernel en direccion x*/
  for(i=0; i<kc; i++){
    hx[i]=-(i-kc/2)*(i-kc/2);
    hx[i]/=sigma*sigma;
  }
  math_dexp(hx,hx,kc);

  /*Generamos el kernel en direccion x*/
  for(i=0; i<kr; i++){
    hy[i]=-(i-kr/2)*(i-kr/2);
    hy[i]/=sigma*sigma;
  }
  math_dexp(hy,hy,kr);

  convolution_dnormalized(f,g,hx,hy,nr,nc,kr,kc);

  free(hx);
  free(hy);

  return 0;
}

int demod_sft(float *f, float *g, int ir, int ic,
	      int nr, int nc)
{
  register int i;
  float pseed[3];
  float gseed[3];

  /*Corregimos el renglon y la columna inicial para que no se
    encuentre en el borden de la imagen*/
  if(ir==0)
    ir++;
  else if(ir==nr-1)
    ir--;
  if(ic==0)
    ic++;
  else if(ic==nc-1)
    ic--;
  /*Fin de la corrección*/

  /*Establecemos la semilla inicial*/
  gseed[0]=g[IDX(ic-1,ir,nr)];
  gseed[1]=g[IDX(ic,ir,nr)];
  gseed[2]=g[IDX(ic+1,ir,nr)];
  pseed[1]=demod_calc_sacos(g[IDX(ic,ir,nr)]);

  demod_set_sseed(gseed,pseed);
  f[IDX(ic-1,ir,nr)]=pseed[0];
  f[IDX(ic,ir,nr)]=pseed[1];
  f[IDX(ic+1,ir,nr)]=pseed[2];
  /*Fin semilla inicial*/

  /*Demodulamos el renglón*/
  demod_demodula_srow(f,g,ir,ic,nc,nr);

  /*Demodulamos columna por columna*/
  for(i=0; i<nc; i++){
    gseed[0]=g[IDX(i,ir-1,nr)];
    gseed[1]=g[IDX(i,ir,nr)];
    gseed[2]=g[IDX(i,ir+1,nr)];
    pseed[1]=f[IDX(i,ir,nr)];

    demod_set_sseed(gseed,pseed);
    f[IDX(i,ir-1,nr)]=pseed[0];
    f[IDX(i,ir+1,nr)]=pseed[2];

    demod_demodula_scolumn(f,g,i,ir,nr);
  }

  return 0;
}

int demod_dft(double *f, double *g, int ir, int ic,
	      int nr, int nc)
{
  register int i;
  double pseed[3];
  double gseed[3];

  /*Corregimos el renglon y la columna inicial para que no se
    encuentre en el borden de la imagen*/
  if(ir==0)
    ir++;
  else if(ir==nr-1)
    ir--;
  if(ic==0)
    ic++;
  else if(ic==nc-1)
    ic--;
  /*Fin de la corrección*/

  /*Establecemos la semilla inicial*/
  gseed[0]=g[IDX(ic-1,ir,nr)];
  gseed[1]=g[IDX(ic,ir,nr)];
  gseed[2]=g[IDX(ic+1,ir,nr)];
  pseed[1]=demod_calc_dacos(g[IDX(ic,ir,nr)]);

  demod_set_dseed(gseed,pseed);
  f[IDX(ic-1,ir,nr)]=pseed[0];
  f[IDX(ic,ir,nr)]=pseed[1];
  f[IDX(ic+1,ir,nr)]=pseed[2];
  /*Fin semilla inicial*/

  /*Demodulamos el renglón*/
  demod_demodula_drow(f,g,ir,ic,nc,nr);

  /*Demodulamos columna por columna*/
  for(i=ic; i<nc; i++){
    gseed[0]=g[IDX(i,ir-1,nr)];
    gseed[1]=g[IDX(i,ir,nr)];
    gseed[2]=g[IDX(i,ir+1,nr)];
    pseed[1]=f[IDX(i,ir,nr)];

    demod_set_dseed(gseed,pseed);
    f[IDX(i,ir-1,nr)]=pseed[0];
    f[IDX(i,ir+1,nr)]=pseed[2];

    demod_demodula_dcolumn(f,g,i,ir,nr);
  }
  for(i=ic-1; i>=0; i--){
    gseed[0]=g[IDX(i,ir-1,nr)];
    gseed[1]=g[IDX(i,ir,nr)];
    gseed[2]=g[IDX(i,ir+1,nr)];
    pseed[1]=f[IDX(i,ir,nr)];

    demod_set_dseed(gseed,pseed);
    f[IDX(i,ir-1,nr)]=pseed[0];
    f[IDX(i,ir+1,nr)]=pseed[2];

    demod_demodula_dcolumn(f,g,i,ir,nr);
  }

  return 0;
}

int filter_sreg1(float *f, float *g,
		 const float lambda, float* max_error,
		 float* max_iters, const int nr, const int nc)
{
  const int n=nr*nc;
  float* restrict p=malloc(n*sizeof(float));
  float* restrict pp=malloc(n*sizeof(float));
  float* restrict r=malloc(n*sizeof(float));
  float* restrict rr=malloc(n*sizeof(float));
  float* restrict z=malloc(n*sizeof(float));
  float* restrict zz=malloc(n*sizeof(float));

  if(p==NULL || pp==NULL || r==NULL || rr==NULL || z==NULL || zz==NULL)
    return 1;

  optim_filt_reg1_slinbcg(f,g,p,pp,r,rr,z,zz,lambda,*max_error,*max_iters,
			  max_iters,max_error,nr,nc);

  free(p);
  free(pp);
  free(r);
  free(rr);
  free(z);
  free(zz);

  return 0;
}

int filter_sreg2(float *f, float *g,
		 const float lambda, float* max_error,
		 float* max_iters, const int nr, const int nc)
{
  const int n=nr*nc;
  float* restrict p=malloc(n*sizeof(float));
  float* restrict pp=malloc(n*sizeof(float));
  float* restrict r=malloc(n*sizeof(float));
  float* restrict rr=malloc(n*sizeof(float));
  float* restrict z=malloc(n*sizeof(float));
  float* restrict zz=malloc(n*sizeof(float));

  if(p==NULL || pp==NULL || r==NULL || rr==NULL || z==NULL || zz==NULL)
    return 1;

  optim_filt_reg2_slinbcg(f,g,p,pp,r,rr,z,zz,lambda,*max_error,*max_iters,
			  max_iters,max_error,nr,nc);

  free(p);
  free(pp);
  free(r);
  free(rr);
  free(z);
  free(zz);

  return 0;
}

int filter_dreg1(double *f, double *g,
		 const double lambda, double* max_error,
		 double* max_iters, const int nr, const int nc)
{
  const int n=nr*nc;
  double* restrict p=malloc(n*sizeof(double));
  double* restrict pp=malloc(n*sizeof(double));
  double* restrict r=malloc(n*sizeof(double));
  double* restrict rr=malloc(n*sizeof(double));
  double* restrict z=malloc(n*sizeof(double));
  double* restrict zz=malloc(n*sizeof(double));

  if(p==NULL || pp==NULL || r==NULL || rr==NULL || z==NULL || zz==NULL)
    return 1;

  optim_filt_reg1_dlinbcg(f,g,p,pp,r,rr,z,zz,lambda,*max_error,*max_iters,
			  max_iters,max_error,nr,nc);

  free(p);
  free(pp);
  free(r);
  free(rr);
  free(z);
  free(zz);

  return 0;
}

int filter_dreg2(double *f, double *g,
		 const double lambda, double* max_error,
		 double* max_iters, const int nr, const int nc)
{
  const int n=nr*nc;
  double* restrict p=malloc(n*sizeof(double));
  double* restrict pp=malloc(n*sizeof(double));
  double* restrict r=malloc(n*sizeof(double));
  double* restrict rr=malloc(n*sizeof(double));
  double* restrict z=malloc(n*sizeof(double));
  double* restrict zz=malloc(n*sizeof(double));

  if(p==NULL || pp==NULL || r==NULL || rr==NULL || z==NULL || zz==NULL)
    return 1;

  optim_filt_reg2_dlinbcg(f,g,p,pp,r,rr,z,zz,lambda,*max_error,*max_iters,
			  max_iters,max_error,nr,nc);

  free(p);
  free(pp);
  free(r);
  free(rr);
  free(z);
  free(zz);

  return 0;

}

int filter_sqreg1(float *fi, float* psi, float *g, const float lambda, 
		 const float wx, const float wy,
		 float* max_error, int* max_iters, 
		 const int nr, const int nc)
{
  const int n=nr*nc;
  int i;
  float* restrict p=malloc(n*sizeof(float));
  float* restrict pp=malloc(n*sizeof(float));
  float* restrict r=malloc(n*sizeof(float));
  float* restrict rr=malloc(n*sizeof(float));
  float* restrict z=malloc(n*sizeof(float));
  float* restrict zz=malloc(n*sizeof(float));
  float* restrict pi=malloc(n*sizeof(float));
  float* restrict ppi=malloc(n*sizeof(float));
  float* restrict ri=malloc(n*sizeof(float));
  float* restrict rri=malloc(n*sizeof(float));
  float* restrict zi=malloc(n*sizeof(float));
  float* restrict zzi=malloc(n*sizeof(float));
  float* restrict gi=malloc(n*sizeof(float));

  if(p==NULL || pp==NULL || r==NULL || rr==NULL || z==NULL || zz==NULL ||
     pi==NULL || ppi==NULL || ri==NULL || rri==NULL || zi==NULL || zzi==NULL ||
     gi==NULL)
    return 1;

  for(i=0; i<n; i++)
    gi[i]=0.;

  optim_filt_qreg1_slinbcg(fi,psi,g,gi,p,pi,pp,ppi,r,ri,rr,rri,z,zi,
			   zz,zzi,lambda,wx,wy,*max_error,*max_iters,
			   max_iters,max_error,nr,nc);

  free(p);
  free(pp);
  free(r);
  free(rr);
  free(z);
  free(zz);
  free(pi);
  free(ppi);
  free(ri);
  free(rri);
  free(zi);
  free(zzi);

  return 0;
}

int filter_dqreg1(double *fi, double* psi, double *g, const double lambda, 
		  const double wx, const double wy,
		  double* max_error, int* max_iters, const int method,
		  const int nr, const int nc)
{
  const int n=nr*nc;
  register int i,j;
  double* restrict p;
  double* restrict pp;
  double* restrict r;
  double* restrict rr;
  double* restrict z;
  double* restrict zz;
  double* restrict pi;
  double* restrict ppi;
  double* restrict ri;
  double* restrict rri;
  double* restrict zi;
  double* restrict zzi;
  double* restrict gi;
  double* restrict aux;

  if(method==0){ /*Usa graadiente conjugado*/
    p=malloc(n*sizeof(double));
    pp=malloc(n*sizeof(double));
    r=malloc(n*sizeof(double));
    rr=malloc(n*sizeof(double));
    z=malloc(n*sizeof(double));
    zz=malloc(n*sizeof(double));
    pi=malloc(n*sizeof(double));
    ppi=malloc(n*sizeof(double));
    ri=malloc(n*sizeof(double));
    rri=malloc(n*sizeof(double));
    zi=malloc(n*sizeof(double));
    zzi=malloc(n*sizeof(double));
    gi=malloc(n*sizeof(double));
    aux=malloc(n*sizeof(double));

    if(p==NULL || pp==NULL || r==NULL || rr==NULL || z==NULL || zz==NULL ||
       pi==NULL || ppi==NULL || ri==NULL || rri==NULL || zi==NULL || 
       zzi==NULL || gi==NULL)
      return 1;

    for(i=0; i<nr; i++)
      for(j=0; j<nc; j++){
	size_t k;
	k=j*nr+i;
	aux[k]=0;
	if(i>0)
	  aux[k]+=2*(g[k]-g[k-1]);
	if(i+1 < nr)
	  aux[k]+=2*(g[k]-g[k+1]);
	if(j>0)
	  aux[k]+=2*(g[k]-g[k-nr]);
	if(j+1 < nc)
	  aux[k]+=2*(g[k]-g[k+nr]);
      }

    for(i=0; i<n; i++){
      gi[i]=0.;
    }
    optim_filt_qreg1_dlinbcg(fi,psi,aux,gi,p,pi,pp,ppi,r,ri,rr,rri,z,zi,
			     zz,zzi,lambda,wx,wy,*max_error,*max_iters,
			     max_iters,max_error,nr,nc);
    free(p);
    free(pp);
    free(r);
    free(rr);
    free(z);
    free(zz);
    free(pi);
    free(ppi);
    free(ri);
    free(rri);
    free(zi);
    free(zzi);
    free(aux);

  }else /*Usa gauss seidel*/
    optim_filt_qreg1_dgs(fi,psi,g,lambda,wx,wy,max_error,max_iters,nr,nc);

  return 0;
}


int filter_local_dqreg1(double *fi, double* psi, double *g, 
			const double lambda, 
			const double wx, const double wy, 
			double* max_error, int* max_iters,
			const int N, const int ir, const int ic,
			const int nr, const int nc)
{
  double *restrict s=calloc(nr*nc,sizeof(double));
  double *restrict sa=calloc(nr*nc,sizeof(double));


  if(s==NULL)
    return 1;

  optim_local_filt_qreg1_dgs(fi,psi,sa,sa,g,s,sa,lambda,0,wx,wy,max_error,
			     max_iters,ir,ic,nr,nc,N);


  free(s);
  free(sa);

  return 0;
}

int demod_local_dft(double *f, double *g, double* sp, 
		    int ir, int ic,
		    const int nr, const int nc, const int N)
{
  register int i;
  double pseed[3];
  double gseed[3];
  const int n=N/2;
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;
  register int k;

  /*Corregimos el renglon y la columna inicial para que no se
    encuentre en el borden de la imagen*/
  if(ir==0)
    ir++;
  else if(ir==nr-1)
    ir--;
  if(ic==0)
    ic++;
  else if(ic==nc-1)
    ic--;
  /*Fin de la corrección*/

  k=IDX(ic,ir,nr);

  /*Establecemos la semilla inicial*/
  //if(sp[k]!=1 && sp[k-nr]!=1 && sp[k+nr]!=1){
    gseed[0]=g[k-nr];
    gseed[1]=g[k];
    gseed[2]=g[k+nr];
    pseed[1]=demod_calc_dacos(g[k]);

    demod_set_dseed(gseed,pseed);
    f[k-nr]=pseed[0];
    f[k]=pseed[1];
    f[k+nr]=pseed[2];
    sp[k-nr]=1;
    sp[k]=1;
    sp[k+nr]=1;
  //}
  /*Fin semilla inicial*/

  /*Demodulamos el renglón*/
  demod_demodula_dsubrow(f,g,sp,ir,ic,lic,lsc,nr);

  /*Demodulamos columna por columna*/
  for(i=ic; i<lsc; i++){
    k=IDX(i,ir,nr);
    //if(sp[k-1]!=1 || sp[k+1]!=1){
      gseed[0]=g[k-1];
      gseed[1]=g[k];
      gseed[2]=g[k+1];
      pseed[1]=f[k];
      
      demod_set_dseed(gseed,pseed);
      f[k-1]=pseed[0];
      f[k+1]=pseed[2];
      sp[k-1]=1;
      sp[k+1]=1;
    //}
    demod_demodula_dsubcolumn(f,g,sp,i,ir,lir,lsr,nr);
  }
  for(i=ic-1; i>=lic; i--){
    k=IDX(i,ir,nr);
    //if(sp[k-1]!=1 || sp[k+1]!=1){
      gseed[0]=g[k-1];
      gseed[1]=g[k];
      gseed[2]=g[k+1];
      pseed[1]=f[k];
      
      demod_set_dseed(gseed,pseed);
      f[k-1]=pseed[0];
      f[k+1]=pseed[2];
      sp[k-1]=1;
      sp[k+1]=1;
    //}
    demod_demodula_dsubcolumn(f,g,sp,i,ir,lir,lsr,nr);
  }
  return 0;
}

int demod_robust_dft(const double *p, const double *g, double wx, double wy,
		     const double lambda, const double mu, const double error,
		     const int ir, const int ic, 
		     const int N, const int nr, const int nc)
{
  int i,j;

  void* follower;
  double *restrict fi=calloc(nr*nc,sizeof(double));
  double *restrict psi=calloc(nr*nc,sizeof(double));
  double *restrict s=calloc(nr*nc,sizeof(double));
  double *restrict sp=calloc(nr*nc,sizeof(double));
  double *restrict fia=calloc(nr*nc,sizeof(double));
  double *restrict psia=calloc(nr*nc,sizeof(double));
  double *restrict sa=calloc(nr*nc,sizeof(double));
  const int iters=(N<10)? 10:N;
  double merror;
  int miters;


  if(fi==NULL || psi==NULL || s==NULL || sp==NULL)
    return 1;

  follower=NULL;
  follower_dcreate(&follower,(double*)g,ir,ic,nr,nc);

  merror=error;
  miters=iters;
  optim_local_filt_qreg1_dgs(fi,psi,fia,psia,(double*)g,s,sa,
                         lambda,mu,wx,wy,&merror,&miters,ir,ic,nr,nc,N);
  math_local_datan2((double*)p,sp,psi,fi,ir,ic,N,nr,nc);
  math_sub_dcopy(fi, fia, sp, ir, ic, nr, nc, N);
  math_sub_dcopy(psi, psia, sp, ir, ic, nr, nc, N);
  math_sub_dcopy(s, sa, sp, ir, ic, nr, nc, N);
  
  while(follower_next(follower)!=0){
    follower_get_point(follower,&i,&j);
    
    demod_local_calc_dfreqx((double*)p,sp,fi,psi, &wx, &wy,i,j,nr,nc,N);
    
    merror=error;
    miters=iters;
    optim_local_filt_qreg1_dgs(fi,psi,fia,psia,(double*)g,s,sa,
       lambda,mu,wx,wy,&merror,&miters,i,j,nr,nc,N);
    math_local_datan2((double*)p,sp,psi,fi,i,j,N,nr,nc);

    math_sub_dcopy(fi, fia, sp, i, j, nr, nc, N);
    math_sub_dcopy(psi, psia, sp, i, j, nr, nc, N);
    math_sub_dcopy(s, sa, sp, i, j, nr, nc, N);
  }

  follower_delete(&follower);
  free(fi);
  free(psi);
  free(s);
  free(sp);
  free(fia);
  free(psia);
  free(sa);

  return 0;
}

void fringeproc_get_dmin(double* g, int* ir, int* ic, int nr, int nc)
{
  double min=g[0];
  register int i,j,k;
  *ir=*ic=0;

  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=IDX(j,i,nr);
      if(min>g[k]){
        min=g[k];
        *ir=i;
        *ic=j;
      }
    }
}

void fringeproc_get_dmax(double* g, int* ir, int* ic, int nr, int nc)
{
  double max=g[0];
  register int i,j,k=0;
  *ir=*ic=0;

  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
    k=IDX(j,i,nr);
    if(max<g[k]){
        max=g[k];
        *ir=i;
        *ic=j;
      }
    }
}

void demod_drpt(double* restrict p, double* restrict g, const double lambda,
		const int N, const int initc, const int initr)
{

}
