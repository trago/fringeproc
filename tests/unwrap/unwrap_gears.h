#ifndef UNWRAP_GEARS_H
#define UNWRAP_GEARS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8_t;

float W(float phase);
float calcUnwrapped(const size_t idx, const int x, const int y,
                    const float *phase,
                    const float *uphase,
                    const uint8_t *visited, float tao,
                    const size_t M, const size_t N);

#ifdef __cplusplus
}
#endif

#endif // UNWRAP_GEARS_H
