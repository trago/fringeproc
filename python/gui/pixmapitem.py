import numpy as np
from PyQt4.QtGui import QGraphicsPixmapItem
from PyQt4.QtGui import QPixmap, QImage
from PyQt4.QtCore import QObject, Qt, pyqtSignal
from image import Image

class ObjectPixmapItem(QObject):
    """
    Combenient class created to emit signals.

    Signals:
     * clicked: Emited when user clicks over a item.
       :arguments:
         * pos: A tuple representing the position (x,y)
           :type: tuple
    """
    clicked = pyqtSignal(tuple)

    def __init__(self, parent=None):
        super(ObjectPixmapItem, self).__init__(parent)


class PixmapItem(QGraphicsPixmapItem):
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
    # Object to manage signals in this Pixmap item
    signals = ObjectPixmapItem()

    def __init__(self, obj, parent=None):
        """
        UnwrapPixmapItem(pixmap, parent=None)
        Constructur the pixmap item and activates the event handling when the mouse
        passes over the image.

        Author: Julio C. Estrada
        """
        if isinstance(obj, np.ndarray):
            self._image = Image(obj)
        elif isinstance(obj, QImage):
            self._image = obj
        else:
            raise TypeError(
                  "Data type must be a numpy.ndarray or a PyQt3.QtGui.QImage")

        pixmap = QPixmap.fromImage(self._image)
        super(PixmapItem, self).__init__(pixmap, parent)

        self.setAcceptHoverEvents(True)
        #self.setAcceptedMouseButtons(Qt.LeftButton)s
        self.setCursor(Qt.CrossCursor)

    def __del__(self):
        """
        Destructor.

        Notes:
         * I implemented this function only to avoid a crash when the application
           is closed. I do not know if this is a bug from PyQt4.
        """
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
            self._moveEventHandler(event.pos(), self._image.getData())

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
        
    def getDataImage(self):
        return self._image.getData()

    def mouseReleaseEvent(self, event):
        self.signals.clicked.emit((int(event.pos().x()), int(event.pos().y())))
        #super(UnwrapPixmapItem, self).mouseReleaseEvent(event)
    