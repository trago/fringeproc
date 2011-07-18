/*
** optimization.c
** 
** Made by (Julio Cesar Estrada Rico)
** Login   <julio@localhost>
** 
** Started on  Sat Sep  2 13:57:26 2006 Julio Cesar Estrada Rico
** Last update Fri Oct 20 21:23:28 2006 Julio Cesar Estrada Rico
*/

#include "optimization.h"
#include "mfuntions.h"
#include <stdlib.h>
#include <math.h>
#include <cblas.h>

void optim_filt_reg1_satimes(float* restrict b, float* restrict x,
			     const float lambda, const int nr, const int nc)
{
  register float suma;
  register int i, j;
  register int k;

  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      b[k]= x[k];
      suma=0;
      
      if(i>0)
        suma+= x[k]-x[k-1];
      if(i+1 < nr)
        suma+= x[k]-x[k+1];
      if(j>0)
        suma+= x[k]-x[k-nr];
      if(j+1 < nc)
        suma+= x[k]-x[k+nr];

      b[k]+= lambda*suma;
    }
}

void optim_filt_reg2_satimes(float* restrict b, float* restrict x,
			     const float lambda, const int nr, const int nc)
{
  register float suma;
  register int i, j;
  register int k;
  
  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      b[k]= x[k];
      suma=0;
      
      if(i>0 && i<nr)
        suma-= 2*(x[k-1] - 2*x[k] + x[k+1]);
      if(i+2 < nr)
        suma+= x[k]- 2*x[k+1] + x[k+2];
      if(i-2 >= 0)
	suma+= x[k-2] - 2*x[k-1] + x[k];
      
      if(j>0 && j<nc)
        suma-= 2*(x[k-nr] - 2*x[k] + x[k+nr]);
      if(j+2<nc)
        suma+= x[k] - 2*x[k+nr] + x[k+2*nr];
      if(j-2>=0)
	suma+=x[k-2*nr] - 2*x[k-nr] + x[k];
      
      if(i+1<nr && j-1>=0)
	suma+=x[k]-x[k - nr] - x[k+1]+x[k -nr+1];
      if(i+1<nr && j+1<nc)
	suma+=x[k]-x[k + nr] - x[k+1]+x[k +nr+1];
      if(i-1>=0 && j+1<nc)
	suma+=x[k]-x[k + nr] - x[k-1]+x[k +nr-1];
      if(i-1>=0 && j-1>=0)
	suma+=x[k]-x[k - nr] - x[k-1]+x[k -nr-1];
      
      b[k]+= lambda*suma;
    }
}

void optim_filt_reg1_sasolve(float* restrict x, float* restrict b,
			const float l, const int nr, const int nc)
{
  register int i,j;
  register int k;
  register int nl;
  
  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      nl=0;
      
      if(i>0)
	nl++;
      if(i+1 < nr)
	nl++;
      if(j>0)
	nl++;
      if(j+1 < nc)
	nl++;
      
      x[k]=b[k]/(1+nl*l);
    }
}

void optim_filt_reg2_sasolve(float* restrict x, float* restrict b,
			const float l, const int nr, const int nc)
{
  register int i,j;
  register int k;
  register int nl;
  
  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      nl=0;
      
      if(i>0 && i<nr)
        nl+=4;
      if(i+2 < nr)
        nl++;
      if(i-2 >= 0)
	nl++;

      if(j>0 && j<nc)
        nl+=4;
      if(j+2<nc)
        nl++;
      if(j-2>=0)
	nl++;

      if(i+1<nr && j-1>=0)
	nl++;
      if(i+1<nr && j+1<nc)
	nl++;
      if(i-1>=0 && j+1<nc)
	nl++;
      if(i-1>=0 && j-1>=0)
	nl++;

      x[k]=b[k]/(1+nl*l);
    }
}


void optim_filt_reg1_slinbcg(float* restrict x, float* restrict b,
			     float* restrict p, float* restrict pp,
			     float* restrict r, float* restrict rr,
			     float* restrict z, float* restrict zz,
			     const float lambda, const float tol, 
			     const int itmax, float* restrict iter,
			     float* restrict err, 
			     const int nr, const int nc)
{
  register float ak,akden,bk,bkden=0,bknum,bnrm;
  register int j;
  const int n=nr*nc;

  *iter=0;
  optim_filt_reg1_satimes(r,x,lambda,nr,nc);

  cblas_saxpy(n,-1,b,1,r,1);
  cblas_sscal(n,-1,r,1);
  cblas_scopy(n,r,1,rr,1);

  /*Descomentar esto para obtener la veriente del residuo mínimo del 
    algoritmo.*/
  /*optim_filter_reg1_satimes(rr,r,lambda,nr,nc);*/
  bnrm=cblas_snrm2(n,b,1);
  optim_filt_reg1_sasolve(z,r,lambda,nr,nc);

  while(((*iter)++) <= itmax){
    optim_filt_reg1_sasolve(zz,rr,lambda,nr,nc);
    bknum=cblas_sdot(n,z,1,rr,1);

    if(*iter == 1){
      cblas_scopy(n,z,1,p,1);
      cblas_scopy(n,zz,1,pp,1);
    }
    else{
      bk=bknum/bkden;
      for(j=0; j<n; j++){
	p[j]=bk*p[j]+z[j];
	pp[j]=bk*pp[j]+zz[j];
      }
    }
    bkden=bknum;
    optim_filt_reg1_satimes(z,p,lambda,nr,nc);
    akden=cblas_sdot(n,z,1,pp,1);
    ak=bknum/akden;
    optim_filt_reg1_satimes(zz,pp,lambda,nr,nc);
    for(j=0; j<n; j++){
      x[j]+= ak*p[j];
      r[j]-= ak*z[j];
      rr[j]-= ak*zz[j];
    }
    optim_filt_reg1_sasolve(z,r,lambda,nr,nc);

    *err=cblas_snrm2(n,r,1)/bnrm;
    if(*err <= tol) break;
  }

}


/**************************/

void optim_filt_reg1_datimes(double* restrict b, double* restrict x,
			     const double lambda, const int nr, const int nc)
{
  register double suma;
  register int i, j;
  register int k;

  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      b[k]= x[k];
      suma=0;
      
      if(i>0)
        suma+= x[k]-x[k-1];
      if(i+1 < nr)
        suma+= x[k]-x[k+1];
      if(j>0)
        suma+= x[k]-x[k-nr];
      if(j+1 < nc)
        suma+= x[k]-x[k+nr];

      b[k]+= lambda*suma;
    }
}

void optim_filt_reg2_datimes(double* restrict b, double* restrict x,
			     const double lambda, const int nr, const int nc)
{
  register double suma;
  register int i, j;
  register int k;

  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      b[k]= x[k];
      suma=0;
      
      if(i>0 && i<nr)
        suma-= 2*(x[k-1] - 2*x[k] + x[k+1]);
      if(i+2 < nr)
        suma+= x[k]- 2*x[k+1] + x[k+2];
      if(i-2 >= 0)
	suma+= x[k-2] - 2*x[k-1] + x[k];

      if(j>0 && j<nc)
        suma-= 2*(x[k-nr] - 2*x[k] + x[k+nr]);
      if(j+2<nc)
        suma+= x[k] - 2*x[k+nr] + x[k+2*nr];
      if(j-2>=0)
	suma+=x[k-2*nr] - 2*x[k-nr] + x[k];

      if(i+1<nr && j-1>=0)
	suma+=x[k]-x[k - nr] - x[k+1]+x[k -nr+1];
      if(i+1<nr && j+1<nc)
	suma+=x[k]-x[k + nr] - x[k+1]+x[k +nr+1];
      if(i-1>=0 && j+1<nc)
	suma+=x[k]-x[k + nr] - x[k-1]+x[k +nr-1];
      if(i-1>=0 && j-1>=0)
	suma+=x[k]-x[k - nr] - x[k-1]+x[k -nr-1];

      b[k]+= lambda*suma;
    }
}

void optim_filt_reg1_dasolve(double* restrict x, double* restrict b,
			const double l, const int nr, const int nc)
{
  register int i,j;
  register int k;
  register int nl;
  
  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      nl=0;
      
      if(i>0)
	nl++;
      if(i+1 < nr)
	nl++;
      if(j>0)
	nl++;
      if(j+1 < nc)
	nl++;
      
      x[k]=b[k]/(1+nl*l);
    }
}

void optim_filt_reg2_dasolve(double* restrict x, double* restrict b,
			const double l, const int nr, const int nc)
{
  register int i,j;
  register int k;
  register int nl;
  
  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      nl=0;
      
      if(i>0 && i<nr)
        nl+=4;
      if(i+2 < nr)
        nl++;
      if(i-2 >= 0)
	nl++;

      if(j>0 && j<nc)
        nl+=4;
      if(j+2<nc)
        nl++;
      if(j-2>=0)
	nl++;

      if(i+1<nr && j-1>=0)
	nl++;
      if(i+1<nr && j+1<nc)
	nl++;
      if(i-1>=0 && j+1<nc)
	nl++;
      if(i-1>=0 && j-1>=0)
	nl++;

      x[k]=b[k]/(1+nl*l);
    }
}


void optim_filt_reg1_dlinbcg(double* restrict x, double* restrict b,
			     double* restrict p, double* restrict pp,
			     double* restrict r, double* restrict rr,
			     double* restrict z, double* restrict zz,
			     const double lambda, const double tol, 
			     const int itmax, double* restrict iter,
			     double* restrict err, 
			     const int nr, const int nc)
{
  register double ak,akden,bk,bkden=0,bknum,bnrm;
  register int j;
  const int n=nr*nc;

  *iter=0;
  optim_filt_reg1_datimes(r,x,lambda,nr,nc);

  cblas_daxpy(n,-1,b,1,r,1);
  cblas_dscal(n,-1,r,1);
  cblas_dcopy(n,r,1,rr,1);

  /*Descomentar esto para obtener la veriente del residuo mínimo del 
    algoritmo.*/
  /*optim_filter_reg1_datimes(rr,r,lambda,nr,nc);*/
  bnrm=cblas_dnrm2(n,b,1);
  optim_filt_reg1_dasolve(z,r,lambda,nr,nc);

  while(((*iter)++) <= itmax){
    optim_filt_reg1_dasolve(zz,rr,lambda,nr,nc);
    bknum=cblas_ddot(n,z,1,rr,1);

    if(*iter == 1){
      cblas_dcopy(n,z,1,p,1);
      cblas_dcopy(n,zz,1,pp,1);
    }
    else{
      bk=bknum/bkden;
      for(j=0; j<n; j++){
	p[j]=bk*p[j]+z[j];
	pp[j]=bk*pp[j]+zz[j];
      }
    }
    bkden=bknum;
    optim_filt_reg1_datimes(z,p,lambda,nr,nc);
    akden=cblas_ddot(n,z,1,pp,1);
    ak=bknum/akden;
    optim_filt_reg1_datimes(zz,pp,lambda,nr,nc);
    for(j=0; j<n; j++){
      x[j]+= ak*p[j];
      r[j]-= ak*z[j];
      rr[j]-= ak*zz[j];
    }
    optim_filt_reg1_dasolve(z,r,lambda,nr,nc);

    *err=cblas_dnrm2(n,r,1)/bnrm;
    if(*err <= tol) break;
  }

}


void optim_filt_reg2_slinbcg(float* restrict x, float* restrict b,
			     float* restrict p, float* restrict pp,
			     float* restrict r, float* restrict rr,
			     float* restrict z, float* restrict zz,
			     const float lambda, const float tol, 
			     const int itmax, float* restrict iter,
			     float* restrict err, 
			     const int nr, const int nc)
{
  register float ak,akden,bk,bkden=0,bknum,bnrm;
  register int j;
  const int n=nr*nc;

  *iter=0;
  optim_filt_reg2_satimes(r,x,lambda,nr,nc);

  cblas_saxpy(n,-1,b,1,r,1);
  cblas_sscal(n,-1,r,1);
  cblas_scopy(n,r,1,rr,1);

  /*Descomentar esto para obtener la veriente del residuo mínimo del 
    algoritmo.*/
  /*optim_filter_reg2_satimes(rr,r,lambda,nr,nc);*/
  bnrm=cblas_snrm2(n,b,1);
  optim_filt_reg2_sasolve(z,r,lambda,nr,nc);

  while(((*iter)++) <= itmax){
    optim_filt_reg2_sasolve(zz,rr,lambda,nr,nc);
    bknum=cblas_sdot(n,z,1,rr,1);

    if(*iter == 1){
      cblas_scopy(n,z,1,p,1);
      cblas_scopy(n,zz,1,pp,1);
    }
    else{
      bk=bknum/bkden;
      for(j=0; j<n; j++){
	p[j]=bk*p[j]+z[j];
	pp[j]=bk*pp[j]+zz[j];
      }
    }
    bkden=bknum;
    optim_filt_reg2_satimes(z,p,lambda,nr,nc);
    akden=cblas_sdot(n,z,1,pp,1);
    ak=bknum/akden;
    optim_filt_reg2_satimes(zz,pp,lambda,nr,nc);
    for(j=0; j<n; j++){
      x[j]+= ak*p[j];
      r[j]-= ak*z[j];
      rr[j]-= ak*zz[j];
    }
    optim_filt_reg2_sasolve(z,r,lambda,nr,nc);

    *err=cblas_snrm2(n,r,1)/bnrm;
    if(*err <= tol) break;
  }
}

void optim_filt_reg2_dlinbcg(double* restrict x, double* restrict b,
			     double* restrict p, double* restrict pp,
			     double* restrict r, double* restrict rr,
			     double* restrict z, double* restrict zz,
			     const double lambda, const double tol, 
			     const int itmax, double* restrict iter,
			     double* restrict err, 
			     const int nr, const int nc)
{
  register double ak,akden,bk,bkden=0,bknum,bnrm;
  register int j;
  const int n=nr*nc;

  *iter=0;
  optim_filt_reg2_datimes(r,x,lambda,nr,nc);

  cblas_daxpy(n,-1,b,1,r,1);
  cblas_dscal(n,-1,r,1);
  cblas_dcopy(n,r,1,rr,1);

  /*Descomentar esto para obtener la veriente del residuo mínimo del 
    algoritmo.*/
  /*optim_filter_reg2_datimes(rr,r,lambda,nr,nc);*/
  bnrm=cblas_dnrm2(n,b,1);
  optim_filt_reg2_dasolve(z,r,lambda,nr,nc);

  while(((*iter)++) <= itmax){
    optim_filt_reg2_dasolve(zz,rr,lambda,nr,nc);
    bknum=cblas_ddot(n,z,1,rr,1);

    if(*iter == 1){
      cblas_dcopy(n,z,1,p,1);
      cblas_dcopy(n,zz,1,pp,1);
    }
    else{
      bk=bknum/bkden;
      for(j=0; j<n; j++){
	p[j]=bk*p[j]+z[j];
	pp[j]=bk*pp[j]+zz[j];
      }
    }
    bkden=bknum;
    optim_filt_reg2_datimes(z,p,lambda,nr,nc);
    akden=cblas_ddot(n,z,1,pp,1);
    ak=bknum/akden;
    optim_filt_reg2_datimes(zz,pp,lambda,nr,nc);
    for(j=0; j<n; j++){
      x[j]+= ak*p[j];
      r[j]-= ak*z[j];
      rr[j]-= ak*zz[j];
    }
    optim_filt_reg2_dasolve(z,r,lambda,nr,nc);

    *err=cblas_dnrm2(n,r,1)/bnrm;
    if(*err <= tol) break;
  }
}

void optim_filt_qreg1_satimes(float* restrict br, float* restrict bi,
			      float* restrict xr, float* restrict xi,
			      const float lambda, 
			      const float cx, const float cy,
			      const float sx, const float sy,
			      const int nr, const int nc)
{
  float* restrict pgfi=br;
  float* restrict pgpsi=bi;
  float* restrict pfi=xr;
  float* restrict ppsi=xi;
  register float sumar=0;
  register float sumai=0;
  register int i,j,k;
  
  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;
      pgfi[k]=pgpsi[k]=0;
      sumar=0;
      sumai=0;
	
      if(i>0){
        sumar+= pfi[k] - pfi[k-1]*cy + ppsi[k-1]*sy;
        sumai+=ppsi[k] - ppsi[k-1]*cy - pfi[k-1]*sy;
        pgfi[k]+=pfi[k]-pfi[k-1];
        pgpsi[k]+=ppsi[k]-ppsi[k-1];
      }
      if(i+1 < nr){
        sumar+= pfi[k] - pfi[k+1]*cy - ppsi[k+1]*sy;
        sumai+=ppsi[k] - ppsi[k+1]*cy + pfi[k+1]*sy;
        pgfi[k]+=pfi[k]-pfi[k+1];
        pgpsi[k]+=ppsi[k]-ppsi[k+1];
      }
      if(j>0){
        sumar+= pfi[k] - pfi[k-nr]*cx + ppsi[k-nr]*sx;
        sumai+=ppsi[k] - ppsi[k-nr]*cx - pfi[k-nr]*sx;
        pgfi[k]+=pfi[k]-pfi[k-nr];
        pgpsi[k]+=ppsi[k]-ppsi[k-nr];
      }
      if(j+1 < nc){
        sumar+= pfi[k] - pfi[k+nr]*cx - ppsi[k+nr]*sx;
        sumai+=ppsi[k] - ppsi[k+nr]*cx + pfi[k+nr]*sx;
        pgfi[k]+=pfi[k]-pfi[k+nr];
        pgpsi[k]+=ppsi[k]-ppsi[k+nr];
      }
      pgfi[k]+= lambda*sumar;
      pgpsi[k]+=lambda*sumai;
  }
}

void optim_filt_qreg1_sasolve(float* restrict xr, float* restrict xi,
			     float* restrict br, float* restrict bi,
			     const float l, const int nr, const int nc)
{
  register float nl;
  register int i,j,k;

  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;

      nl=0;
	
      if(i>0)
        nl++;
      if(i+1 < nr)
        nl++;
      if(j>0)
        nl++;
      if(j+1 < nc)
        nl++;
	
      xr[k]=br[k]/(nl+nl*l);
      xi[k]=bi[k]/(nl+nl*l);
    }
}

void optim_filt_qreg1_slinbcg(float* restrict xr, float* restrict xi,
			      float* restrict br, float* restrict bi,
			      float* restrict pr, float* restrict pi,
			      float* restrict ppr, float* restrict ppi,
			      float* restrict rr, float* restrict ri,
			      float* restrict rrr, float* restrict rri,
			      float* restrict zr, float* restrict zi,
			      float* restrict zzr, float* restrict zzi,
			      const float lambda, 
			      const float wx, const float wy,
			      const float tol,
			      const int itmax, int* restrict iter,
			      float* restrict err, 
			      const int nr, const int nc)
{
  register float ak,akden,bk,bkden=0,bknum,bnrm;
  register int j;
  const int n=nr*nc;
  const float sx=sin(wx);
  const float sy=sin(wy);
  const float cx=cos(wx);
  const float cy=cos(wy);

  *iter=0;
  optim_filt_qreg1_satimes(rr,ri,xr,xi,lambda,cx,cy,sx,sy,nr,nc);

  cblas_saxpy(n,-1,br,1,rr,1);
  cblas_saxpy(n,-1,bi,1,ri,1);
  cblas_sscal(n,-1,rr,1);
  cblas_sscal(n,-1,ri,1);
  cblas_scopy(n,rr,1,rrr,1);
  cblas_scopy(n,ri,1,rri,1);

  /*Descomentar esto para obtener la veriente del residuo mínimo del 
    algoritmo.*/
  /*optim_filter_reg1_satimes(rr,r,lambda,nr,nc);*/
  bnrm=sqrt(cblas_sdot(n,br,1,br,1)+cblas_sdot(n,bi,1,bi,1));
  optim_filt_qreg1_sasolve(zr,zi,rr,ri,lambda,nr,nc);

  while(((*iter)++) <= itmax){
    optim_filt_qreg1_sasolve(zzr,zzi,rrr,rri,lambda,nr,nc);
    bknum=cblas_sdot(n,zr,1,rrr,1)+cblas_sdot(n,zi,1,rri,1);

    if(*iter == 1){
      cblas_scopy(n,zr,1,pr,1);
      cblas_scopy(n,zi,1,pi,1);
      cblas_scopy(n,zzr,1,ppr,1);
      cblas_scopy(n,zzi,1,ppi,1);
    }
    else{
      bk=bknum/bkden;
      for(j=0; j<n; j++){
	pr[j]=bk*pr[j]+zr[j];
	pi[j]=bk*pi[j]+zi[j];
	ppr[j]=bk*ppr[j]+zzr[j];
	ppi[j]=bk*ppi[j]+zzi[j];
      }
    }
    bkden=bknum;
    optim_filt_qreg1_satimes(zr,zi,pr,pi,lambda,cx,cy,sx,sy,nr,nc);
    akden=cblas_sdot(n,zr,1,ppr,1)+cblas_sdot(n,zi,1,ppi,1);
    ak=bknum/akden;
    optim_filt_qreg1_satimes(zzr,zzi,ppr,ppi,lambda,cx,cy,sx,sy,nr,nc);
    for(j=0; j<n; j++){
      xr[j]+= ak*pr[j];
      xi[j]+= ak*pi[j];
      rr[j]-= ak*zr[j];
      ri[j]-= ak*zi[j];
      rrr[j]-= ak*zzr[j];
      rri[j]-= ak*zzi[j];
    }
    optim_filt_qreg1_sasolve(zr,zi,rr,ri,lambda,nr,nc);

    *err=sqrt(cblas_sdot(n,rr,1,rr,1)+cblas_sdot(n,ri,1,ri,1))/bnrm;
    if(*err <= tol) break;
  }
}

/*****/

void optim_filt_qreg1_datimes(double* restrict br, double* restrict bi,
			      double* restrict xr, double* restrict xi,
			      const double lambda, 
			      const double cx, const double cy,
			      const double sx, const double sy,
			      const int nr, const int nc)
{
  double* restrict pgfi=br;
  double* restrict pgpsi=bi;
  double* restrict pfi=xr;
  double* restrict ppsi=xi;
  register double sumar=0;
  register double sumai=0;
  register int i,j,k;
  
  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr+i;

      pgfi[k]=pgpsi[k]=0;
      sumar=0;
      sumai=0;
      
      if(i>0){
	sumar+= pfi[k] - pfi[k-1]*cy + ppsi[k-1]*sy;
	sumai+=ppsi[k] - ppsi[k-1]*cy - pfi[k-1]*sy;
	pgfi[k]+=pfi[k]-pfi[k-1];
	pgpsi[k]+=ppsi[k]-ppsi[k-1];
      }
      if(i+1 < nr){
	sumar+= pfi[k] - pfi[k+1]*cy - ppsi[k+1]*sy;
	sumai+=ppsi[k] - ppsi[k+1]*cy + pfi[k+1]*sy;
	pgfi[k]+=pfi[k]-pfi[k+1];
	pgpsi[k]+=ppsi[k]-ppsi[k+1];
      }
      if(j>0){
	sumar+= pfi[k] - pfi[k-nr]*cx + ppsi[k-nr]*sx;
	sumai+=ppsi[k] - ppsi[k-nr]*cx - pfi[k-nr]*sx;
	pgfi[k]+=pfi[k]-pfi[k-nr];
	pgpsi[k]+=ppsi[k]-ppsi[k-nr];
      }
      if(j+1 < nc){
	sumar+= pfi[k] - pfi[k+nr]*cx - ppsi[k+nr]*sx;
	sumai+=ppsi[k] - ppsi[k+nr]*cx + pfi[k+nr]*sx;
	pgfi[k]+=pfi[k]-pfi[k+nr];
	pgpsi[k]+=ppsi[k]-ppsi[k+nr];
      }
      pgfi[k]+= lambda*sumar;
      pgpsi[k]+=lambda*sumai;
    }    
}

void optim_filt_qreg1_dasolve(double* restrict xr, double* restrict xi,
			     double* restrict br, double* restrict bi,
			     const double l, const int nr, const int nc)
{
  register double nl;
  register int i,j,k;

  for(i=0; i<nr; i++)
    for(j=0; j<nc; j++){
      k=j*nr +i;

      nl=0;
	
      if(i>0)
        nl++;
      if(i+1 < nr)
        nl++;
      if(j>0)
        nl++;
      if(j+1 < nc)
        nl++;
	
      xr[k]=br[k]/(nl+nl*l);
      xi[k]=bi[k]/(nl+nl*l);
    }
}

void optim_filt_qreg1_dlinbcg(double* restrict xr, double* restrict xi,
			      double* restrict br, double* restrict bi,
			      double* restrict pr, double* restrict pi,
			      double* restrict ppr, double* restrict ppi,
			      double* restrict rr, double* restrict ri,
			      double* restrict rrr, double* restrict rri,
			      double* restrict zr, double* restrict zi,
			      double* restrict zzr, double* restrict zzi,
			      const double lambda, 
			      const double wx, const double wy,
			      const double tol,
			      const int itmax, int* restrict iter,
			      double* restrict err, 
			      const int nr, const int nc)
{
  register double ak,akden,bk,bkden=0,bknum,bnrm;
  register int j;
  const int n=nr*nc;
  const double sx=sin(wx);
  const double sy=sin(wy);
  const double cx=cos(wx);
  const double cy=cos(wy);

  *iter=0;
  optim_filt_qreg1_datimes(rr,ri,xr,xi,lambda,cx,cy,sx,sy,nr,nc);

  cblas_daxpy(n,-1,br,1,rr,1);
  cblas_daxpy(n,-1,bi,1,ri,1);
  cblas_dscal(n,-1,rr,1);
  cblas_dscal(n,-1,ri,1);
  cblas_dcopy(n,rr,1,rrr,1);
  cblas_dcopy(n,ri,1,rri,1);

  /*Descomentar esto para obtener la veriente del residuo mínimo del 
    algoritmo.*/
  /*optim_filter_reg1_datimes(rr,r,lambda,nr,nc);*/
  bnrm=sqrt(cblas_ddot(n,br,1,br,1)+cblas_ddot(n,bi,1,bi,1));
  optim_filt_qreg1_dasolve(zr,zi,rr,ri,lambda,nr,nc);

  while(((*iter)++) <= itmax){
    optim_filt_qreg1_dasolve(zzr,zzi,rrr,rri,lambda,nr,nc);
    bknum=cblas_ddot(n,zr,1,rrr,1)+cblas_ddot(n,zi,1,rri,1);

    if(*iter == 1){
      cblas_dcopy(n,zr,1,pr,1);
      cblas_dcopy(n,zi,1,pi,1);
      cblas_dcopy(n,zzr,1,ppr,1);
      cblas_dcopy(n,zzi,1,ppi,1);
    }
    else{
      bk=bknum/bkden;
      for(j=0; j<n; j++){
	pr[j]=bk*pr[j]+zr[j];
	pi[j]=bk*pi[j]+zi[j];
	ppr[j]=bk*ppr[j]+zzr[j];
	ppi[j]=bk*ppi[j]+zzi[j];
      }
    }
    bkden=bknum;
    optim_filt_qreg1_datimes(zr,zi,pr,pi,lambda,cx,cy,sx,sy,nr,nc);
    akden=cblas_ddot(n,zr,1,ppr,1)+cblas_ddot(n,zi,1,ppi,1);
    ak=bknum/akden;
    optim_filt_qreg1_datimes(zzr,zzi,ppr,ppi,lambda,cx,cy,sx,sy,nr,nc);
    for(j=0; j<n; j++){
      xr[j]+= ak*pr[j];
      xi[j]+= ak*pi[j];
      rr[j]-= ak*zr[j];
      ri[j]-= ak*zi[j];
      rrr[j]-= ak*zzr[j];
      rri[j]-= ak*zzi[j];
    }
    optim_filt_qreg1_dasolve(zr,zi,rr,ri,lambda,nr,nc);

    *err=sqrt(cblas_ddot(n,rr,1,rr,1)+cblas_ddot(n,ri,1,ri,1))/bnrm;
    if(*err <= tol) break;
  }

}

void optim_filt_qreg1_sgs(float* restrict fi, float* restrict psi,
			  float* restrict g, const float lambda,
			  const float wx, const float wy,
			  float* restrict err, int* restrict iters,
			  const int nr, const int nc)
{
  register int i,j,k;
  register float sumar, sumai, nl, rnrm=0;
  const float tol=*err;
  const int miters=*iters;
  const int n=nr*nc;
  /*const float bnrm=cblas_snrm2(n,g,1);*/
  const float sx=sin(wx);
  const float sy=sin(wy);
  const float cx=cos(wx);
  const float cy=cos(wy);
  float* restrict auxr=malloc(n*sizeof(float));
  float* restrict auxi=malloc(n*sizeof(float));

  *iters=0;

  while(((*iters)++)<miters){

    if(*iters<=1)
      rnrm=sqrt(cblas_sdot(n,g,1,g,1));
    else
      rnrm=sqrt(cblas_sdot(n,fi,1,fi,1) + cblas_sdot(n,psi,1,psi,1));

    for(i=0; i<nr; i++)
      for(j=0; j<nc; j++){
	k=j*nr +i;
	
	auxr[k]=0;
	auxi[k]=0;
	sumar=sumai=nl=0;
	
	if(i>0){
	  sumar-= - fi[k-1]*cy + psi[k-1]*sy;
	  sumai-= - psi[k-1]*cy - fi[k-1]*sy;
	  auxr[k]+=fi[k-1] + 2*(g[k]-g[k-1]);
	  auxi[k]+=psi[k-1];
	  nl++;
	}
	if(i+1 < nr){
	  sumar-= - fi[k+1]*cy - psi[k+1]*sy;
	  sumai-= - psi[k+1]*cy + fi[k+1]*sy;
	  auxr[k]+=fi[k+1] + 2*(g[k]-g[k+1]);
	  auxi[k]+=psi[k+1];
	  nl++;
	}
	if(j>0){
	  sumar-= - fi[k-nr]*cx + psi[k-nr]*sx;
	  sumai-= - psi[k-nr]*cx - fi[k-nr]*sx;
	  auxr[k]+=fi[k-nr] + 2*(g[k]-g[k-nr]);
	  auxi[k]+=psi[k-nr];
	  nl++;
	}
	if(j+1 < nc){
	  sumar-= - fi[k+nr]*cx - psi[k+nr]*sx;
	  sumai-= - psi[k+nr]*cx + fi[k+nr]*sx;
	  auxr[k]+=fi[k+nr] + 2*(g[k]-g[k+nr]);
	  auxi[k]+=psi[k+nr];
	  nl++;
	}
	fi[k]=(auxr[k]+ lambda*sumar)/(nl+nl*lambda);
	psi[k]=(auxi[k]+ lambda*sumai)/(nl+nl*lambda);
      }

    /*cblas_scopy(n,auxr,1,fi,1);
      cblas_scopy(n,auxi,1,psi,1);*/
    rnrm-=sqrt(cblas_sdot(n,fi,1,fi,1) + cblas_sdot(n,psi,1,psi,1));

    *err=fabs(rnrm);
    if(*err<=tol)
      break;
  }
  free(auxr);
  free(auxi);
}

void optim_filt_qreg1_dgs(double* restrict fi, double* restrict psi,
			  double* restrict g, const double lambda,
			  const double wx, const double wy,
			  double* restrict err, int* restrict iters,
			  const int nr, const int nc)
{
  register int i,j,k;
  register double sumar, sumai, nl, rnrm=0;
  const double tol=*err;
  const int miters=*iters;
  const int n=nr*nc;
  /*const double bnrm=cblas_dnrm2(n,g,1);*/
  const double sx=sin(wx);
  const double sy=sin(wy);
  const double cx=cos(wx);
  const double cy=cos(wy);
  double* restrict auxr=malloc(n*sizeof(double));
  double* restrict auxi=malloc(n*sizeof(double));

  *iters=0;

  while(((*iters)++)<miters){

    if(*iters<=1)
      rnrm=sqrt(cblas_ddot(n,g,1,g,1));
    else
      rnrm=sqrt(cblas_ddot(n,fi,1,fi,1) + cblas_ddot(n,psi,1,psi,1));

    for(i=0; i<nr; i++)
      for(j=0; j<nc; j++){
	k=j*nr +i;
	
	auxr[k]=0;
	auxi[k]=0;
	sumar=sumai=nl=0;
	
	if(i>0){
	  sumar-= - fi[k-1]*cy + psi[k-1]*sy;
	  sumai-= - psi[k-1]*cy - fi[k-1]*sy;
	  auxr[k]+=fi[k-1] + 2*(g[k]-g[k-1]);
	  auxi[k]+=psi[k-1];
	  nl++;
	}
	if(i+1 < nr){
	  sumar-= - fi[k+1]*cy - psi[k+1]*sy;
	  sumai-= - psi[k+1]*cy + fi[k+1]*sy;
	  auxr[k]+=fi[k+1] + 2*(g[k]-g[k+1]);
	  auxi[k]+=psi[k+1];
	  nl++;
	}
	if(j>0){
	  sumar-= - fi[k-nr]*cx + psi[k-nr]*sx;
	  sumai-= - psi[k-nr]*cx - fi[k-nr]*sx;
	  auxr[k]+=fi[k-nr] + 2*(g[k]-g[k-nr]);
	  auxi[k]+=psi[k-nr];
	  nl++;
	}
	if(j+1 < nc){
	  sumar-= - fi[k+nr]*cx - psi[k+nr]*sx;
	  sumai-= - psi[k+nr]*cx + fi[k+nr]*sx;
	  auxr[k]+=fi[k+nr] + 2*(g[k]-g[k+nr]);
	  auxi[k]+=psi[k+nr];
	  nl++;
	}
	fi[k]=(auxr[k]+ lambda*sumar)/(nl+nl*lambda);
	psi[k]=(auxi[k]+ lambda*sumai)/(nl+nl*lambda);
      }

    /*cblas_dcopy(n,auxr,1,fi,1);
      cblas_dcopy(n,auxi,1,psi,1);*/
    rnrm-=sqrt(cblas_ddot(n,fi,1,fi,1) + cblas_ddot(n,psi,1,psi,1));

    *err=fabs(rnrm);
    if(*err<=tol)
      break;
  }
  free(auxr);
  free(auxi);
}


void optim_local_filt_qreg1_dgs(double* restrict fi, double* restrict psi,
				 double* restrict fia, double* restrict psia,
				 double* restrict g, 
				 double* restrict s, double* restrict sa,
				 const double lambda, const double mu, 
				 const double wx, const double wy,
				 double* restrict err, int* restrict iters,
				 const int ir, const int ic,
				 const int nr, const int nc, const int N)
{
  register int i,j,k;
  register double sumar, sumai, nl, ns, rnrm, sfi, spsi;
  const double tol=*err;
  const int miters=*iters;
  const int n=N/2;
  const double sx=sin(wx);
  const double sy=sin(wy);
  const double cx=cos(wx);
  const double cy=cos(wy);
  double* restrict auxr=malloc(nr*nc*sizeof(double));
  double* restrict auxi=malloc(nr*nc*sizeof(double));
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;

  *iters=0;

  while(((*iters)++)<miters){

    if(*iters<=1)
      rnrm=sqrt(math_sub_ddot(g,g,s,ir,ic,nr,nc,N));
    else
      rnrm=sqrt(math_sub_ddot(fi,fi,s,ir,ic,nr,nc,N) + 
		math_sub_ddot(psi,psi,s,ir,ic,nr,nc,N));

    for(i=lir; i<lsr; i++)
      for(j=lic; j<lsc; j++){
	k=j*nr +i;

	sumar=sumai=nl=0;
	auxr[k]=auxi[k]=0;

	sfi=fia[k]*sa[k];
	spsi=psia[k]*sa[k];
	ns=sa[k];
	
	if(lir>0){
	  if(i-1 > lir || s[k-1]==1){
	    sumar-= - fi[k-1]*cy + psi[k-1]*sy;
	    sumai-= - psi[k-1]*cy - fi[k-1]*sy;
	    auxr[k]+=fi[k-1] + 2*(g[k]-g[k-1]);
	    auxi[k]+=psi[k-1];
	    nl++;
	  }
	}
	else
	  if(i-1 > lir){
	    sumar-= - fi[k-1]*cy + psi[k-1]*sy;
	    sumai-= - psi[k-1]*cy - fi[k-1]*sy;
	    auxr[k]+=fi[k-1] + 2*(g[k]-g[k-1]);
	    auxi[k]+=psi[k-1];
	    nl++;
	  }
	
	if(lsr+1 < nr){
	  if(i+1 < lsr || s[k+1]==1){
	    sumar-= - fi[k+1]*cy - psi[k+1]*sy;
	    sumai-= - psi[k+1]*cy + fi[k+1]*sy;
	    auxr[k]+=fi[k+1] + 2*(g[k]-g[k+1]);
	    auxi[k]+=psi[k+1];
	    nl++;
	  }
	}
	else
	  if(i+1 < lsr){
	    sumar-= - fi[k+1]*cy - psi[k+1]*sy;
	    sumai-= - psi[k+1]*cy + fi[k+1]*sy;
	    auxr[k]+=fi[k+1] + 2*(g[k]-g[k+1]);
	    auxi[k]+=psi[k+1];
	    nl++;
	  }

	if(lic>0){
	  if(j>lic || s[k-nr]==1){
	    sumar-= - fi[k-nr]*cx + psi[k-nr]*sx;
	    sumai-= - psi[k-nr]*cx - fi[k-nr]*sx;
	    auxr[k]+=fi[k-nr] + 2*(g[k]-g[k-nr]);
	    auxi[k]+=psi[k-nr];
	    nl++;
	  }
	}
	else
	  if(j>lic /*|| s[k-nr]==1*/){
	    sumar-= - fi[k-nr]*cx + psi[k-nr]*sx;
	    sumai-= - psi[k-nr]*cx - fi[k-nr]*sx;
	    auxr[k]+=fi[k-nr] + 2*(g[k]-g[k-nr]);
	    auxi[k]+=psi[k-nr];
	    nl++;
	  }

	if(lsc+1<nr){
	  if(j+1 < lsc || s[k+nr]==1){
	    sumar-= - fi[k+nr]*cx - psi[k+nr]*sx;
	    sumai-= - psi[k+nr]*cx + fi[k+nr]*sx;
	    auxr[k]+=fi[k+nr] + 2*(g[k]-g[k+nr]);
	    auxi[k]+=psi[k+nr];
	    nl++;
	  }
	}
	else
	  if(j+1 < lsc /*|| s[k+nr]==1*/){
	    sumar-= - fi[k+nr]*cx - psi[k+nr]*sx;
	    sumai-= - psi[k+nr]*cx + fi[k+nr]*sx;
	    auxr[k]+=fi[k+nr] + 2*(g[k]-g[k+nr]);
	    auxi[k]+=psi[k+nr];
	    nl++;
	  }

	fi[k]=(auxr[k]+ lambda*sumar + mu*sfi)/(nl+ mu*ns + nl*lambda);
	psi[k]=(auxi[k]+ lambda*sumai +mu*spsi)/(nl+mu*ns + nl*lambda);
      }

    rnrm-=sqrt(math_sub_ddot(fi,fi,s,ir,ic,nr,nc,N) + 
		math_sub_ddot(psi,psi,s,ir,ic,nr,nc,N));

    *err=fabs(rnrm);
    if(*err<=tol)
      break;

  }

  for(i=lir; i<lsr; i++)
    for(j=lic; j<lsc; j++){
      k=j*nr +i;
      if(s[k]==0)
	s[k]=1;
    }

  free(auxr);
  free(auxi);
}


void optim_local_filt_qreg1_gradient(
    double* restrict fi, double* restrict psi,
         double* restrict fia, double* restrict psia,
         double* restrict g, 
         double* restrict s, double* restrict sa,
         const double lambda, const double mu, const double alpha, 
         const double wx, const double wy,
         double* restrict err, int* restrict iters,
         const int ir, const int ic,
         const int nr, const int nc, const int N)
{
  register int i,j,k;
  register double sumar, sumai, rnrm=0;
  const double tol=*err;
  const int miters=*iters;
  const int n=N/2;
  const double sx=sin(wx);
  const double sy=sin(wy);
  const double cx=cos(wx);
  const double cy=cos(wy);
  double* restrict auxr=malloc(nr*nc*sizeof(double));
  double* restrict auxi=malloc(nr*nc*sizeof(double));
  const int lir=(ir-n>=0)? ir-n:0;
  const int lsr=(ir+n<nr)? ir+n:nr;
  const int lic=(ic-n>=0)? ic-n:0;
  const int lsc=(ic+n<nc)? ic+n:nc;

  *iters=0;

  while(((*iters)++)<miters){

    if(*iters<=1)
      rnrm=sqrt(math_sub_ddot(g,g,NULL,ir,ic,nr,nc,N));
    //else
    //  rnrm=sqrt(math_sub_ddot(fi,fi,s,ir,ic,nr,nc,N) + 
	//	math_sub_ddot(psi,psi,s,ir,ic,nr,nc,N));
  
    for(i=lir; i<lsr; i++)
      for(j=lic; j<lsc; j++){
        k=j*nr +i;

        auxr[k]=(fi[k]-fia[k])*mu*sa[k];
        auxi[k]=(psi[k]-psia[k])*mu*sa[k];
        sumar=sumai=0;

        if(lir>0){
          if(i-1 > lir || s[k-1]==1){
            sumar= fi[k] - fi[k-1]*cy + psi[k-1]*sy;
            sumai+=psi[k] - psi[k-1]*cy - fi[k-1]*sy;
            auxr[k]+= fi[k]-fi[k-1] - 2*(g[k]-g[k-1]);
            auxi[k]+=psi[k]-psi[k-1];
          }
        }
        else
          if(i-1 > lir){
            sumar+= fi[k]- fi[k-1]*cy + psi[k-1]*sy;
            sumai+=psi[k]- psi[k-1]*cy - fi[k-1]*sy;
            auxr[k]+= fi[k]-fi[k-1] - 2*(g[k]-g[k-1]);
            auxi[k]+=psi[k]-psi[k-1];
          }
        
        if(lsr+1 < nr){
          if(i+1 < lsr || s[k+1]==1){
            sumar+= fi[k]- fi[k+1]*cy - psi[k+1]*sy;
            sumai+=psi[k]- psi[k+1]*cy + fi[k+1]*sy;
            auxr[k]+= fi[k]-fi[k+1] - 2*(g[k]-g[k+1]);
            auxi[k]+=psi[k]-psi[k+1];
          }
        }
        else
          if(i+1 < lsr){
            sumar+= fi[k]- fi[k+1]*cy - psi[k+1]*sy;
            sumai+=psi[k]- psi[k+1]*cy + fi[k+1]*sy;
            auxr[k]+= fi[k]-fi[k+1] - 2*(g[k]-g[k+1]);
            auxi[k]+=psi[k]-psi[k+1];
          }

        if(lic>0){
          if(j>lic || s[k-nr]==1){
            sumar+= fi[k]- fi[k-nr]*cx + psi[k-nr]*sx;
            sumai+=psi[k]- psi[k-nr]*cx - fi[k-nr]*sx;
            auxr[k]+= fi[k]-fi[k-nr] - 2*(g[k]-g[k-nr]);
            auxi[k]+=psi[k]-psi[k-nr];
          }
        }
        else
          if(j>lic || s[k-nr]==1){
            sumar+= fi[k]- fi[k-nr]*cx + psi[k-nr]*sx;
            sumai+=psi[k]- psi[k-nr]*cx - fi[k-nr]*sx;
            auxr[k]+= fi[k]-fi[k-nr] - 2*(g[k]-g[k-nr]);
            auxi[k]+=psi[k]-psi[k-nr];
          }

        if(lsc+1<nr){
          if(j+1 < lsc || s[k+nr]==1){
            sumar+= fi[k]- fi[k+nr]*cx - psi[k+nr]*sx;
            sumai+=psi[k]- psi[k+nr]*cx + fi[k+nr]*sx;
            auxr[k]+= fi[k]-fi[k+nr] - 2*(g[k]-g[k+nr]);
            auxi[k]+=psi[k]-psi[k+nr];
          }
        }
        else
          if(j+1 < lsc || s[k+nr]==1){
            sumar+= fi[k]- fi[k+nr]*cx - psi[k+nr]*sx;
            sumai+=psi[k]- psi[k+nr]*cx + fi[k+nr]*sx;
            auxr[k]+= fi[k]-fi[k+nr] - 2*(g[k]-g[k+nr]);
            auxi[k]+=psi[k]-psi[k+nr];
          }
        auxr[k]+=sumar*lambda;
        auxi[k]+=sumai*lambda;
      }
    for(i=lir; i<lsr; i++)
      for(j=lic; j<lsc; j++){
        k=j*nr+i;
        fi[k]-= alpha*auxr[k];
        psi[k]-=alpha*auxi[k];
      }

    *err=sqrt(math_sub_ddot(auxr,auxr,NULL,ir,ic,nr,nc,N) + 
    math_sub_ddot(auxi,auxi,NULL,ir,ic,nr,nc,N))/rnrm;

    //*err=fabs(rnrm);
    if(*err<=tol)
      break;
  }

  for(i=lir; i<lsr; i++)
    for(j=lic; j<lsc; j++){
      k=j*nr +i;
      if(s[k]==0)
        s[k]=1;
    }

  free(auxr);
  free(auxi);
}

