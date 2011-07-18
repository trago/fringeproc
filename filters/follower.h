#ifndef FOLLOWER_H_
#define FOLLOWER_H_

#ifdef __cplusplus
extern "C"{
#endif

int follower_dcreate(void** follower, double* I, const int ir,
                     const int ic, const int nr, const int nc);

void follower_delete(void** follower);

void follower_get_point(void* follower, int* ir, int* ic);

int follower_next(void* follower);

void follower_get_qmap(void* follower, double* __restrict map, const int nr, 
                      const int nc);

#ifdef __cplusplus
}
#endif

#endif
