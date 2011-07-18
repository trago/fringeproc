#ifndef LINALG_H
#define LINALG_H

#ifdef __cplusplus
extern "C" {
#endif

void sum_ss(const float *op1, const float *op2, float *res, const int N);
void sum_sd(const float *op1, const double *op2, double *res, const int N);
void sum_sc(const float *op1, const char *op2, float *res, const int N);
void sum_sn(const float *op1, const int *op2, float *res, const int N);
void sum_ds(const double *op1, const float *op2, double *res, const int N);
void sum_dd(const double *op1, const double *op2, double *res, const int N);
void sum_dc(const double *op1, const char *op2, double *res, const int N);
void sum_dn(const double *op1, const int *op2, double *res, const int N);
void sum_cs(const char *op1, const float *op2, float *res, const int N);
void sum_cd(const char *op1, const double *op2, double *res, const int N);
void sum_cc(const char *op1, const char *op2, char *res, const int N);
void sum_cn(const char *op1, const int *op2, int *res, const int N);
void sum_ns(const int *op1, const float *op2, float *res, const int N);
void sum_nd(const int *op1, const double *op2, double *res, const int N);
void sum_nc(const int *op1, const char *op2, int *res, const int N);
void sum_nn(const int *op1, const int *op2, int *res, const int N);

void diff_ss(const float *op1, const float *op2, float *res, const int N);
void diff_sd(const float *op1, const double *op2, double *res, const int N);
void diff_sc(const float *op1, const char *op2, float *res, const int N);
void diff_sn(const float *op1, const int *op2, float *res, const int N);
void diff_ds(const double *op1, const float *op2, double *res, const int N);
void diff_dd(const double *op1, const double *op2, double *res, const int N);
void diff_dc(const double *op1, const char *op2, double *res, const int N);
void diff_dn(const double *op1, const int *op2, double *res, const int N);
void diff_cs(const char *op1, const float *op2, float *res, const int N);
void diff_cd(const char *op1, const double *op2, double *res, const int N);
void diff_cc(const char *op1, const char *op2, char *res, const int N);
void diff_cn(const char *op1, const int *op2, int *res, const int N);
void diff_ns(const int *op1, const float *op2, float *res, const int N);
void diff_nd(const int *op1, const double *op2, double *res, const int N);
void diff_nc(const int *op1, const char *op2, int *res, const int N);
void diff_nn(const int *op1, const int *op2, int *res, const int N);

void prod_ss(const float *op1, const float *op2, float *res, const int N);
void prod_sd(const float *op1, const double *op2, double *res, const int N);
void prod_sc(const float *op1, const char *op2, float *res, const int N);
void prod_sn(const float *op1, const int *op2, float *res, const int N);
void prod_ds(const double *op1, const float *op2, double *res, const int N);
void prod_dd(const double *op1, const double *op2, double *res, const int N);
void prod_dc(const double *op1, const char *op2, double *res, const int N);
void prod_dn(const double *op1, const int *op2, double *res, const int N);
void prod_cs(const char *op1, const float *op2, float *res, const int N);
void prod_cd(const char *op1, const double *op2, double *res, const int N);
void prod_cc(const char *op1, const char *op2, char *res, const int N);
void prod_cn(const char *op1, const int *op2, int *res, const int N);
void prod_ns(const int *op1, const float *op2, float *res, const int N);
void prod_nd(const int *op1, const double *op2, double *res, const int N);
void prod_nc(const int *op1, const char *op2, int *res, const int N);
void prod_nn(const int *op1, const int *op2, int *res, const int N);

void div_ss(const float *op1, const float *op2, float *res, const int N);
void div_sd(const float *op1, const double *op2, double *res, const int N);
void div_sc(const float *op1, const char *op2, float *res, const int N);
void div_sn(const float *op1, const int *op2, float *res, const int N);
void div_ds(const double *op1, const float *op2, double *res, const int N);
void div_dd(const double *op1, const double *op2, double *res, const int N);
void div_dc(const double *op1, const char *op2, double *res, const int N);
void div_dn(const double *op1, const int *op2, double *res, const int N);
void div_cs(const char *op1, const float *op2, float *res, const int N);
void div_cd(const char *op1, const double *op2, double *res, const int N);
void div_cc(const char *op1, const char *op2, char *res, const int N);
void div_cn(const char *op1, const int *op2, int *res, const int N);
void div_ns(const int *op1, const float *op2, float *res, const int N);
void div_nd(const int *op1, const double *op2, double *res, const int N);
void div_nc(const int *op1, const char *op2, int *res, const int N);
void div_nn(const int *op1, const int *op2, int *res, const int N);

void esc_sum_ss(const float *arr, const float esc, float *res, const int N);

#ifdef __cplusplus
}
#endif

#endif
