from PySide.QtGui import QGraphicsPixmapItem
from PySide.QtCore import QEvent
from PySide.QtCore import Qt

class UnwrapPixmapItem(QGraphicsPixmapItem):
  """
  Pixmap item used to display an image into a graphics scene.
  
  Here one can set the callback function that handles the event when the mouse
  passes over the image.
  
  Author: Julio C. Estrada
  """
  # This is the event handler function.
  _moveEventHandler = None  
  
  def __init__(self, pixmap, parent=None):
    """
    UnwrapPixmapItem(pixmap, parent=None)
    Constructur the pixmap item and activates the event handling when the mouse
    passes over the image.
    
    Author: Julio C. Estrada
    """
    super(UnwrapPixmapItem, self).__init__(pixmap, parent)
    self.setAcceptHoverEvents(True)
    self.setCursor(Qt.CrossCursor)
    
    
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