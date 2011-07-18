#include "follower.h"
#include "seguidor.h"
#include "matrix.h"
#include <cstdlib>

int follower_dcreate(void** follower, double* I, const int ir,
                     const int ic, const int nr, const int nc)
{
  fringe::Matrix mat(I,nr,nc);
  if (*follower==0){
    *follower=new fringe::Seguidor(mat,ir,ic,8);
  }
  if(*follower==0)
    return 1;
  return 0;  
}

void follower_delete(void** follower)
{
  fringe::Seguidor* seg=(fringe::Seguidor*)*follower;
  delete seg;
  *follower=NULL;
}

void follower_get_point(void* follower, int* ir, int* ic)
{
  fringe::Seguidor* seg=(fringe::Seguidor*)follower;
  *ir=seg->get_r();
  *ic=seg->get_c();
}

int follower_next(void* follower)
{
  fringe::Seguidor* seg=(fringe::Seguidor*)follower;
  return seg->siguiente(); 
}

void follower_get_qmap(void* follower, double* __restrict__ map, const int nr, 
                      const int nc)
{
  fringe::Seguidor* seg=(fringe::Seguidor*) follower;
  fringe::Matrix mat;

  mat=seg->get_qmap();
  for(int i=0; i<nr; i++)
    for(int j=0; j<nc; j++)
      map[j*nr+i]=mat[j][i];
}
