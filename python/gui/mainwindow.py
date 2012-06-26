from PyQt4.QtGui import QMainWindow
from ui_mainwin import Ui_MainGUI

class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self._ui = Ui_MainGUI()
        self._ui.setupUi(self)

    def connectSignals(self):
        """
        Connects the signals and slots of menu actions and defines its
        constrains. 

          :author: Julio C. Estrada <julio@cio.mx>
        """
        self._mnuFileClose.triggered.connect(self._onClose)
        self._mnuFileOpen.triggered.connect(self._onOpen)
        self._mnuFileOpenMask.triggered.connect(self._onOpenMask)
        self._mnuFileQuit.triggered.connect(self._onQuit)
        self._mnuFileSave.triggered.connect(self._onSave)
        self._mnuPhaseUnwrapping.triggered.connect(self._onPhaseUnwrapping)
        self._mnuPhaseDemodulation.triggered.connect(self._onPhaseDemodulation)

    def setActionConstrains():
        pass

    def _onClose(self):
        pass

    def _onOpen(self):
        pass

    def _onOpenMask(self):
        pass

    def _onQuit(self):
        pass

    def _onSave(self):
        pass

    def _onPhaseUnwrapping(self):
        pass

    def _onPhaseDemodulation(self):
        pass
