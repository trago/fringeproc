from PyQt4.QtGui import QDialog, QGraphicsItem
from PyQt4.QtCore import pyqtSlot, pyqtSignal
from dlgprogressresult import ProgressResult
from unwrapprocess import Unwrap
import ui_dlgunwrap as dlgUi


class DlgUnwrap(QDialog):
    _nSize = 9
    _tau = 0.09
    _smooth = 13
    _pixel = (0,0)
    _data = None
    _ui =None
    _process = None
    _progress = None
    
    process_done = pyqtSignal(int)

    def __init__(self, parent=None):
        super(DlgUnwrap, self).__init__(parent)
        self._progress = ProgressResult(parent)
        self._process = Unwrap(self._progress, self)
        parent.setProcess(self._process)
        pixmapItem = parent.getScene().items()[0]
        self._data = pixmapItem.getImage().getData()
        
        self._ui = dlgUi.Ui_DlgUnwrap()
        self._ui.setupUi(self)
        
        self._process.finished.connect(self.onUnwrapFinished)
        self._ui.pushFromImage.clicked.connect(self.onFromImage)
        self.finished.connect(self.onFinished)

    @pyqtSlot(int)
    def onUnwrapFinished(self, result):
        """
        Called when the phase unwrapping process has done.
        """
        self.process_done.emit(result)
        
    @pyqtSlot(int)
    def onFinished(self, result):
        """
        Called when the user press 'accept' or 'cancel'
        """
        gui = self.parent()
        scene = gui.getScene()
        # Se supone que solo hay un item y que es un pixmap item.
        pixmapItem = scene.items()[0]
        pixmapItem.setFlag(QGraphicsItem.ItemIsSelectable, False)
        
        if result==1:
            self._process.showProgress()
            self._process.start()
        
        #pixmapItem.signals.clicked.disconnect(self.onSelectPixel)        
        self.process_done.emit(result)

    @pyqtSlot()
    def onFromImage(self):
        """
        Called when user selects 'From image' in order to select a pixel from
        the image shown.
        """
        gui = self.parent()
        scene = gui.getScene()
        # Se supone que solo hay un item y que es un pixmap item.
        pixmapItem = scene.items()[0]
        pixmapItem.setFlag(QGraphicsItem.ItemIsSelectable, True)
        pixmapItem.signals.clicked.connect(self.onSelectPixel)

        self.hide()

    @pyqtSlot(tuple)
    def onSelectPixel(self, pixel):
        """
        Called when the user gives cleck over the PixelItem shown in the 
        scene.
        """
        self._ui.linePixelX.setText(str(pixel[0]))
        self._ui.linePixelY.setText(str(pixel[1]))
        self.exec_()

    def getParameters(self):
        params = {
            'nSize':self._nSize,
            'tau':self._tau,
            'smooth':self._smooth,
            'pixel':self._pixel,
            'data':self._data
        }
        return params

    def accept(self):
        """
        Called when the user press 'accept'.
        """
        ui = self._ui
        self._nSize = int(ui.lineNeighbor.text())
        self._pixel = (int(ui.linePixelX.text()), int(ui.linePixelY.text()))
        self._tau = float(ui.lineTau.text())
        self._smooth = float(ui.lineSmooth.text())
        
                
        super(DlgUnwrap, self).accept()