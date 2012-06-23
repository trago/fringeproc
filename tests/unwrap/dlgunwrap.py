import ui_dlgunwrap as dlgUi
from PyQt4.QtGui import QDialog, QGraphicsItem
from PyQt4.QtCore import pyqtSlot, pyqtSignal

class DlgUnwrap(QDialog):
  _nSize = 9
  _tau = 0.09
  _smooth = 13
  _pixel = (0,0)
  _ui =None
  
  
  
  def __init__(self, parent=None):
    super(DlgUnwrap, self).__init__(parent)
    
    self._ui = dlgUi.Ui_DlgUnwrap()
    self._ui.setupUi(self)
    
    self._ui.pushFromImage.clicked.connect(self.onFromImage)
    self.finished.connect(self.onFinished)
    
  @pyqtSlot(int)
  def onFinished(self, result):
    gui = self.parent()
    scene = gui.getScene()
    # Se supone que solo hay un item y que es un pixmap item.
    pixmapItem = scene.items()[0]
    if(result==1):
      print "Phase unwrapping"
      
    pixmapItem.setFlag(QGraphicsItem.ItemIsSelectable, False)
    try:
      pixmapItem.signals.clicked.disconnect(self.onSelectPixel)
    except TypeError:
      pass
      
  @pyqtSlot()
  def onFromImage(self):
    gui = self.parent()
    scene = gui.getScene()
    # Se supone que solo hay un item y que es un pixmap item.
    pixmapItem = scene.items()[0]
    pixmapItem.setFlag(QGraphicsItem.ItemIsSelectable, True)
    pixmapItem.signals.clicked.connect(self.onSelectPixel)
    
    self.hide()
    print "OK"
    
  @pyqtSlot(tuple)
  def onSelectPixel(self, pixel):
    self._ui.linePixelX.setText(str(pixel[0]))
    self._ui.linePixelY.setText(str(pixel[1]))
    self.exec_()
    
  def getParameters(self):
    params = {
      'nSize':self._nSize,
      'tau':self._tau,
      'smooth':self._smooth,
      'pixel':self._pixel
      }
    return params
  
  def accept(self):
    ui = self._ui
    self._nSize = int(ui.lineNeighbor.text())
    self._pixel = (int(ui.linePixelX.text()), int(ui.linePixelY.text()))
    self._tau = float(ui.lineTau.text())
    self._smooth = float(ui.lineSmooth.text())
    
    super(DlgUnwrap, self).accept()