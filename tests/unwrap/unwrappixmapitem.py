from PySide.QtGui import QGraphicsPixmapItem
from PySide.QtCore import QEvent

class UnwrapPixmapItem(QGraphicsPixmapItem):
  def __init__(self, pixmap, parent=None):
    super(UnwrapPixmapItem, self).__init__(pixmap, parent)
    self.setAcceptHoverEvents(True)
    
  def hoverMoveEvent(self, event):
    print event.pos()
        