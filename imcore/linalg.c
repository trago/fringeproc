#include "linalg.h"

void sum_ss(const float *restrict op1, const float *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_sd(const float *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_sc(const float *restrict op1, const char *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_sn(const float *restrict op1, const int *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}

void sum_ds(const double *restrict op1, const float *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_dd(const double *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_dc(const double *restrict op1, const char *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_dn(const double *restrict op1, const int *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_cs(const char *restrict op1, const float *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_cd(const char *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_cc(const char *restrict op1, const char *restrict op2, 
	    char *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_cn(const char *restrict op1, const int *restrict op2, 
	    int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_ns(const int *restrict op1, const float *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_nd(const int *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_nc(const int *restrict op1, const char *restrict op2, 
	    int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}
void sum_nn(const int *restrict op1, const int *restrict op2, 
	    int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] + op2[i];
}

void diff_ss(const float *restrict op1, const float *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_sd(const float *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_sc(const float *restrict op1, const char *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_sn(const float *restrict op1, const int *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_ds(const double *restrict op1, const float *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_dd(const double *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_dc(const double *restrict op1, const char *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_dn(const double *restrict op1, const int *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_cs(const char *restrict op1, const float *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_cd(const char *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_cc(const char *restrict op1, const char *restrict op2, 
	     char *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_cn(const char *restrict op1, const int *restrict op2, 
	     int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_ns(const int *restrict op1, const float *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_nd(const int *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_nc(const int *restrict op1, const char *restrict op2, 
	     int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void diff_nn(const int *restrict op1, const int *restrict op2, 
	     int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] - op2[i];
}
void prod_ss(const float *restrict op1, const float *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] *  op2[i];
}
void prod_sd(const float *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_sc(const float *restrict op1, const char *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_sn(const float *restrict op1, const int *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_ds(const double *restrict op1, const float *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_dd(const double *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_dc(const double *restrict op1, const char *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_dn(const double *restrict op1, const int *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_cs(const char *restrict op1, const float *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_cd(const char *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_cc(const char *restrict op1, const char *restrict op2, 
	     char *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_cn(const char *restrict op1, const int *restrict op2, 
	     int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_ns(const int *restrict op1, const float *restrict op2, 
	     float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_nd(const int *restrict op1, const double *restrict op2, 
	     double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_nc(const int *restrict op1, const char *restrict op2, 
	     int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}
void prod_nn(const int *restrict op1, const int *restrict op2, 
	     int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] * op2[i];
}

void div_ss(const float *restrict op1, const float *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_sd(const float *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_sc(const float *restrict op1, const char *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_sn(const float *restrict op1, const int *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_ds(const double *restrict op1, const float *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_dd(const double *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_dc(const double *restrict op1, const char *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_dn(const double *restrict op1, const int *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_cs(const char *restrict op1, const float *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_cd(const char *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_cc(const char *restrict op1, const char *restrict op2, 
	    char *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_cn(const char *restrict op1, const int *restrict op2, 
	    int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_ns(const int *restrict op1, const float *restrict op2, 
	    float *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_nd(const int *restrict op1, const double *restrict op2, 
	    double *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_nc(const int *restrict op1, const char *restrict op2, 
	    int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}
void div_nn(const int *restrict op1, const int *restrict op2, 
	    int *restrict res, const int N)
{
  int i;
  for(i=0; i<N; i++)
    res[i] = op1[i] / op2[i];
}

