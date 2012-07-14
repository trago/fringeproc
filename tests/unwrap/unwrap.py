#!/usr/bin/env python
from optparse import OptionParser
import numpy as np
import fringeproc as fproc
import cv2
import sys
import os

usage = "usage: %prog [options] phase_file"

parser = OptionParser(usage=usage)
parser.add_option("-t", "--tao", help="Bandwidth of the IIR system", 
                  type='float', dest='tao', default=0.11, metavar='REAL')
parser.add_option("-N", "--neighborhood_size", 
                  help="The neighborhood size that is processed around each pixel",
                  type='int', dest='N', default=13, metavar='INT')
parser.add_option("-m", "--mask", 
                  help="Mask for determining the region of interest",
                  type='string', dest='mask', metavar="FILE")
parser.add_option("-s", "--sigma", 
                  help="Bandwidth of the gaussian filter applyed to obtain the path",
                  type='int', dest='sigma', default=15, metavar='INT')
parser.add_option("-x", "--col", help="The 'x' coordenate of the starting pixel",
                  type='int', dest='x', metavar='INT')            
parser.add_option("-y", "--row", help="The 'y' coordenate of the starting pixel",
                  type='int', dest='y', metavar='INT')
parser.add_option("-o", "--output", help="The file name where output is stored",
                  type='int', dest='outfile', metavar='FILE')


(options, args) = parser.parse_args(sys.argv[1:])

def readFltFile(fname):
    iter = 0
    data = None
    try:
        with open(fname) as f:
            try:
                M=0
                N=0
                i=0; j=0
                for line in f:
                    if iter == 0:
                        M = float(line)
                    elif iter == 1:
                        N = float(line)
                    elif iter == 2:
                        data = np.zeros((M,N), dtype=float)
                    else:
                        data[i,j%N] = float(line)
                        j += 1
                        if j%N == 0:
                            i += 1
                    iter += 1
            except ValueError:
                print 'Error: Data file %s is wrong or it does not exists!' % fname
                exit(1)
    except IOError:
        print 'Error: There is not such file'
        exit(1)
    return data

def writeFltFile(fname, data):
    try:
        f = open(fname,'w')
        M = str(data.shape[0])
        N = str(data.shape[1])
        f.write(M+'\n')
        f.write(N+'\n')
        for i in xrange(0,data.shape[0]):
            for j in xrange(0,data.shape[1]):
                val = str(data[i,j])
                f.write(val+'\n')
    except IOError:
        print 'Error: I can not write in oput file.'
        exit(1)

if len(args) !=1:
    parser.error("The program needs the file name of the wrapped phase")

fname, ext = os.path.splitext(args[0])
if ext == '.flt':
    print "---> Reading wrapped phase file"
    wphase = readFltFile(args[0])
    print "---> Done."
else:
    wphase = cv2.imread(args[0])
if wphase == None:
    parser.error("Data file format not recognized or the file does not exists")

if options.mask != None:
    print "---> Reading mask file"
    mask = cv2.imread(options.mask, 0)
    if mask == None:
        print "Error: the mask file can not be read"
        exit(1)
    print "---> Done."
else:
    mask = np.ones(wphase.shape)
if mask.shape != wphase.shape:
    parser.error("Tha mask size must be equal to the phase size")

if options.x == None:
    x = wphase.shape[1]/2
else:
    x = options.x
if options.y == None:
    y = wphase.shape[0]/2
else:
    y = options.y
    
wphase = wphase.astype(np.dtype('d')) # tipo double
mask = mask.astype(np.dtype('d')) # tipo double
a = np.min(mask)
b = np.max(mask)
cv2.normalize(wphase, wphase, np.pi, -np.pi, cv2.NORM_MINMAX)
if b-a == 0:
    mask = mask/b
else:
    cv2.normalize(mask, mask, 1, 0, cv2.NORM_MINMAX)
mask = mask.astype(np.dtype('i1')) # tipo char (int8)
print mask.dtype
print wphase.dtype
wphase = wphase*mask

unwraper = fproc.Unwrap(wphase, options.tao, options.sigma, options.N)
unwraper.setPixel((x,y))
unwraper.setMask(mask)

pixelsTotal = np.sum(mask)
pixelCont = 0

print "---> Executing phase unwrapping process"
for iter in [0,1]:
    while unwraper.runInteractive(6000):
        pixelCont += 4100
        percent = pixelCont/(2.0*pixelsTotal)
        print "...> {0}% processed".format(int(percent*100))
    unwraper.setPixel((x,y))
    unwraper.setTao(0.017)
print "---> Done."
up = unwraper.getOutput().copy()
wp = np.arctan2(np.sin(up),np.cos(up))

cv2.normalize(wphase, wphase, 0, 1, cv2.NORM_MINMAX)
cv2.imshow("wrapped", wphase)
cv2.normalize(up, up, 0, 1, cv2.NORM_MINMAX)
cv2.imshow("unwrapped", up)
cv2.normalize(wp, wp, 0, 1, cv2.NORM_MINMAX)
cv2.imshow("rewrapped", wp)
cv2.waitKey(0)