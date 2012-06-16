from PySide.QtGui import QImage
import numpy as np

class UnwrapImage(QImage):
  def __init__(self, np_array):
    self._array = np_array.copy()
    self._data = None
    self._fromC1(self._array)
    
  def _fromC1(self, array):
    if(array.dtype == np.float32 or array.dtype == np.float64):
      a = array.min()
      b = array.max()
      if(a==0 and b==1):
        array=array*255
      else:
        array = 255*(b-array)/(b-a)

    array = array.astype(np.uint32)
    h,w = array.shape
    rgbarray = np.zeros((h,w,3),dtype=np.uint32)
    (rgbarray[:,:,0], rgbarray[:,:,1], rgbarray[:,:,2]) = (array, array, array)
    data = (255 << 24 | rgbarray[:,:,0] << 16 
            | rgbarray[:,:,1] << 8 | rgbarray[:,:,2])
    self._array = array
    self._data = data
    
    super(UnwrapImage, self).__init__(data, w, h, self.Format_RGB32)
    
  def getArray(self):
    return self._array/255.0
    