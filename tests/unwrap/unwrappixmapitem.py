from PySide.QtGui import QGraphicsPixmapItem
from PySide.QtCore import QEvent
from PySide.QtCore import Qt

class UnwrapPixmapItem(QGraphicsPixmapItem):
  def __init__(self, pixmap, parent=None):
    super(UnwrapPixmapItem, self).__init__(pixmap, parent)
    self.setAcceptHoverEvents(True)
    self.setCursor(Qt.CrossCursor)
    
    self._moveEventhandler = None
    
  def hoverMoveEvent(self, event):
    if(self._moveEventhandler is not None):
      self._moveEventhandler(event.pos())
    
  def setMoveEventHandler(self, handler):
    self._moveEventhandler = handler