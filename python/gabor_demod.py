import numpy as np
from gabor import Scanner, DemodGabor
import cv2
from matplotlib import pylab as pl


def peaks(M,N):
  Y,X = np.ogrid[-3:3:M*1j, -3:3:N*1j]
  
  p = (1 - X/2.0 + X**5 + Y**3)*np.exp(-(X**2+Y**2))
  
  return p

def gen_gaborKernel(w, sigma):
  N = int(sigma*3)
  C = 1.0/(sigma*np.sqrt(2*np.pi));
  n = np.r_[-N:N+1]

  h = C*np.exp(-n**2/(2*sigma**2))*np.exp(1j*w*n)
  
  return h

def runInteractive(gabor, scan):
  cont = 0
  while(gabor.runInteractive(scan)):
    if cont%50 == 0:
      pixel = scan.getPosition()
      wx = ffx[pixel]
      wy = ffy[pixel]
      sx = np.fabs(1.5708/wx); sy = np.fabs(1.5708/wy)
      
      kernelSize = gabor.getKernelSize()
      sx = kernelSize if sx>kernelSize else 1 if sx<1 else sx
      sy = kernelSize if sy>kernelSize else 1 if sy<1 else sy
      hx = np.matrix(gen_gaborKernel(wx, sx))
      hy = np.matrix(gen_gaborKernel(wy, sy)).transpose()
      h = np.array(hy*hx)

      hr = cv2.normalize(np.real(h), 1, 0, cv2.NORM_MINMAX)
      cv2.imshow('Kernel', hr)
      fr = cv2.normalize(gabor.getFr(), 1, 0, cv2.NORM_MINMAX)
      cv2.imshow('Out', fr)
      cv2.waitKey(32)
  
    cont = cont+1


P = peaks(256,256)*23
I = np.cos(P) + np.random.randn(256,256)

pixel = (I.shape[1]/3, I.shape[0]/2) #Starting pixel
wu = 0.7 # Frequency x seed
wv = 0.7 # Frequency y seed

gabor = DemodGabor(I)
gabor.setIters(3).setKernelSize(3).setMaxfq(np.pi/2).setMinfq(0.1).setTau(0.3)
gabor.setSeedIters(21)
gabor.setScanMinf(.01);
gabor.setCombFreqs(True).setCombSize(5);
gabor.setStartPixel(pixel).setFreqSeed(wu, wv);

ffx = gabor.getWx()
ffy = gabor.getWy()
fr = gabor.getFr()
fi = gabor.getFi()

scan = Scanner(ffx, ffy, pixel);
scan.setFreqMin(.9);
scan.updateFreqMin(True);

      
pl.imshow(I, cmap=pl.cm.gray)
pl.show()
  