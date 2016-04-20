import numpy as np
from gabor import Scanner, DemodGabor
import cv2
# from matplotlib import pylab as pl


def peaks(M, N):
    X, Y = np.ogrid[-3:3:M*1j, -3:3:N*1j]

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
    pixel = scan.getPosition()
    kernelSize = gabor.getKernelSize()
    while gabor.runInteractive(scan):

        if cont%500 == 0:
            pixel = scan.getPosition()
            wx = gabor.getWxAtPoint(pixel)
            wy = gabor.getWyAtPoint(pixel)
            sx = np.fabs(2*np.pi/wx); sy = np.fabs(2*np.pi/wy)

            sx = kernelSize if sx>kernelSize else 1 if sx<1 else sx
            sy = kernelSize if sy>kernelSize else 1 if sy<1 else sy
            hx = np.matrix(gen_gaborKernel(wx, sx))
            hy = np.matrix(gen_gaborKernel(wy, sy)).transpose()
            h = np.array(hy*hx)

            hr = np.real(h)
            hr = cv2.normalize(hr, None, 0, 1, cv2.NORM_MINMAX)
            cv2.imshow('Kernel', hr)

            fr = gabor.getFr()
            fr = cv2.normalize(fr, None, 0, 1, cv2.NORM_MINMAX)
            cv2.imshow('Out', fr)

            cv2.waitKey(32)

        cont += 1

shape = (256,256)
P = peaks(*shape)*23
I = np.cos(P) + 0.*np.random.randn(*shape)

# shape[0] es y
# shape[1] es x
pixel = (int(I.shape[1]/2), int(I.shape[0]/2)) #Starting pixel
# pixel = (128,85)
wu = 0.7 # Frequency x seed
wv = 0.7 # Frequency y seed

gabor = DemodGabor(I)
gabor.setIters(1).setKernelSize(22).setMaxfq(np.pi/2).setMinfq(0.01).setTau(0.3)
gabor.setSeedIters(5)
gabor.setScanMinf(.01)
gabor.setCombFreqs(True).setCombSize(5)
gabor.setStartPixel(pixel).setFreqSeed(wu, wv)

ffx = gabor.getWx()
ffy = gabor.getWy()
fr = gabor.getFr()
fi = gabor.getFi()

scan = Scanner(ffx, ffy, pixel)
scan.setFreqMin(.7)
scan.updateFreqMin(True)

runInteractive(gabor, scan)

# pl.imshow(I, cmap=pl.cm.gray)
# pl.show()
