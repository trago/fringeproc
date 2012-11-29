#include <Eigen/Dense>
#include <imcore/unwrap.h>
#include "c_unwrap.h"

struct global_unwrap{
  global_unwrap(){
    unwrapper = NULL;
  }

  ~global_unwrap(){
    if(unwrapper != NULL)
      delete unwrapper;
    unwrapper = NULL;
  }

  void release(){
    if(unwrapper != NULL)
      delete unwrapper;
    unwrapper=NULL;
  }

  bool initialized(){
    return unwrapper != NULL;
  }

  void initialize(const Eigen::ArrayXXf& wphase, const float tau,
                  const float sigma, const int x, const int y, const int N){
    release();
    unwrapper = new Unwrap(wphase, tau, sigma, N);
    Eigen::Array2i pixel;
    pixel<< x,y;
    unwrapper->setPixel(pixel);
  }

  Unwrap* unwrapper;

}g_unwrap;

void unwrap_run(float* wphase, float* uphase,
                const int nr, const int nc, const float tau,
                const float sigma, const int x, const int y, const int N)
{
  Eigen::Map<Eigen::ArrayXXf> Wphase(wphase, nr, nc);
  Eigen::Map<Eigen::ArrayXXf> Uphase(uphase, nr, nc);

  g_unwrap.initialize(Wphase, tau, sigma, x, y, N);

  while(g_unwrap.unwrapper->runInteractive(800*800));
  Uphase = g_unwrap.unwrapper->getOutput();

  g_unwrap.release();
}

void unwrap_initialize(float* wphase, float* uphase,
                       const int nr, const int nc, const float tau,
                       const float sigma, const int x, const int y, const int N)
{
  Eigen::Map<Eigen::ArrayXXf> Wphase(wphase, nr, nc);
  g_unwrap.initialize(Wphase, tau, sigma, x, y, N);
}

void unwrap_finalize()
{
  g_unwrap.release();
}

void unwrap_interactive(float* unwrap, int nr, int nc, int iters)
{
  Eigen::Map<Eigen::ArrayXXf> Uphase(unwrap, nr, nc);

  while(g_unwrap.unwrapper->runInteractive(iters));
  Uphase = g_unwrap.unwrapper->getOutput();
}
