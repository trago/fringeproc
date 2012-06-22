from unwrapimage import UnwrapImage

import numpy as np
from PyQt4.QtGui import QGraphicsPixmapItem
from PyQt4.QtGui import QPixmap, QImage
from PyQt4.QtCore import Qt

class UnwrapPixmapItem(QGraphicsPixmapItem):
  """
  Pixmap item used to display an image into a graphics scene.
  
  Here one can set the callback function that handles the event when the mouse
  passes over the image.
  
  Author: Julio C. Estrada
  """
  # This is the event handler function.
  _moveEventHandler = None
  # The QImage asociated to this PixmapItem
  _image = None
  
  def __init__(self, obj, parent=None):
    """
    UnwrapPixmapItem(pixmap, parent=None)
    Constructur the pixmap item and activates the event handling when the mouse
    passes over the image.
    
    Author: Julio C. Estrada
    """
    if isinstance(obj, np.ndarray):
      self._image = UnwrapImage(obj)
      pixmap = QPixmap.fromImage(self._image)
      super(UnwrapPixmapItem, self).__init__(pixmap, parent)
    else:
      super(UnwrapPixmapItem, self).__init__(obj, parent)
      
    self.setAcceptHoverEvents(True)
    self.setCursor(Qt.CrossCursor)
    
  def __del__(self):
    pass
    
  def hoverMoveEvent(self, event):
    """
    hoverMoveEvent(event)
    Called when mouse passes over the image.
    
    Parameters:
     - event, is the object data of the mouse event.
     
    Author: Julio C. Estrada
    """
    if(self._moveEventHandler is not None):
      self._moveEventHandler(event.pos())
    
  def setMoveEventHandler(self, handler):
    """
    setMoveEventHandler(handler)
    Sets the handler for the event when the mouse passes over the image.
    
    Parameters:
     - handler, is the reference to the callback function defined as
       
       def callback(pos):
         ...
         
    Author: Julio C. Estrada
      
    """
    self._moveEventHandler = handler
    
  def getImage(self):
    return self._image
  
  def setImage(self, data):
    if isinstance(data, np.ndarray):
      self._image = UnwrapImage(data)
    elif isinstance(data, QImage):
      self._image = data
    else:
      raise TypeError('Only numpy array or QImage is accepted')
    
    pixmap = QPixmap.fromImage(self._image)
    self.setPixmap(pixmap)
      