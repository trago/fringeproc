from PyQt4.QtGui import QImage
import numpy as np

class UnwrapImage(QImage):
  """
  QImage that loads data from a numpy array.
  
  Use this class to create a QImage from a numpy array using the constructor.
  It only works for gray scale images.
  
  Author Julio C. Estrada
  """
  
  # Keeps the image data that is maped to unsigned integers
  _array = None
  # The three chanels image data obtained from _array
  _data = None
  # The original data
  _image = None
  
  
  def __init__(self, np_array):
    """
    UnwrapImage(numpy_array).
    Creates a QImage from the given numpay array.
    
    Parameters:
     - np_array is the numpy array having the data.
     
    Author: Julio C. Estrada.
    """
    self._image = np_array.copy()
    self._array = np_array.copy()
    
    self._fromC1(self._array)
   
  def _fromC1(self, array):
    """
    _fromC1(array)
    Creates the QImage using an array representing gray scaled values.
    
    Parameters:
     - array, is the array data
    """
    if(array.dtype == np.float32 or array.dtype == np.float64):
      a = array.min()
      b = array.max()
      if(a==0 and b==1):
        array=array*255
      else:
        array = 255*(array-a)/(b-a)

    array = array.astype(np.uint32)
    h,w = array.shape
    rgbarray = np.zeros((h,w,3),dtype=np.uint32)
    (rgbarray[:,:,0], rgbarray[:,:,1], rgbarray[:,:,2]) = (array, array, array)
    data = (255 << 24 | rgbarray[:,:,0] << 16 
            | rgbarray[:,:,1] << 8 | rgbarray[:,:,2])
    self._array = array
    self._data = data
    
    super(UnwrapImage, self).__init__(data, w, h, self.Format_RGB32)
    
  def getDataF(self, min=0, max=1):
    """
    getDataF()
    Returns the image data as real values.
    
    Author: Julio C. Estrada
    """
    if self._image.dtype != np.float:
      data = self._image.astype(np.float)
    a = data.min()
    b = data.max()
    return (data-a)/(b-a)
  
  def getDataU(self):
    """
    getDataU()
    Returns the image data as unsigned integers.
    
    Author: Julio C. Estrada
    """
    return self._array.copy()

  def getData(self, flag=''):
    """
    getData(flag='')
    Returns the original image data.
    
    If flag=='reference' it returns the reference to the image data. It returns
    a copy by default.
    
    Author: Julio C. Estrada
    """
    if flag=='':
      return self._image.coy()
    if flag=='reference':
      return self._image