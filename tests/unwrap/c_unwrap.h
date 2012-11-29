#ifndef C_UNWRAP_H
#define C_UNWRAP_H

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

void unwrap_run(float* wphase, float* uphase,
                const int nr, const int nc, const float tau,
                const float sigma, const int x, const int y, const int N);

void unwrap_initialize(float* wphase, float* uphase,
                       const int nr, const int nc, const float tau,
                       const float sigma, const int x, const int y, const int N);

void unwrap_finalize();

void unwrap_interactive(float* unwrap, int nr, int nc, int iters);


#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

#endif // C_UNWRAP_H
