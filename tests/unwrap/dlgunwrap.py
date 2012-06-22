import ui_dlgunwrap as dlgUi
from PyQt4.QtGui import QDialog

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