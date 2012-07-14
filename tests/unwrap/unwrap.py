import numpy as np
import fringeproc as fproc
import cv2

x,y = np.ogrid[-128:127,-128:127]
phi = 0.001*(x**2 + y**2)

wp = np.arctan2(np.sin(phi), np.cos(phi))


unwraper = fproc.Unwrap(wp, 0.7, 3, 9)
unwraper.setPixel((30,30))

while unwraper.runInteractive(256):
    up = unwraper.getOutput().copy()
    up = np.cos(up)
    cv2.normalize(up, up, 0, 1, cv2.NORM_MINMAX)
    cv2.imshow("unwrapped", up)
    cv2.waitKey(32)
    

cv2.normalize(wp, wp, 0, 1, cv2.NORM_MINMAX)
cv2.imshow("wrapped", wp)
cv2.normalize(up, up, 0, 1, cv2.NORM_MINMAX)
cv2.imshow("unwrapped", np.cos(up))
cv2.waitKey(0)