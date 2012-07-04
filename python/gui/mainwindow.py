from PyQt4.QtGui import QMainWindow
from PyQt4.QtCore import pyqtSlot
from ui_mainwin import Ui_MainGUI
from constrains import Constrains
from filedialog import FileDialog

class MainWindow(QMainWindow):
    _state = None
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self._ui = Ui_MainGUI()
        self._ui.setupUi(self)
        self._state = Constrains()

    def connectSignals(self):
        """
        Connects the signals and slots of menu actions and defines its
        constrains. 

          :author: Julio C. Estrada <julio@cio.mx>
        """
        self._ui._mnuFileClose.triggered.connect(self._onClose)
        self._ui._mnuFileOpen.triggered.connect(self._onOpen)
        self._ui._mnuFileOpenMask.triggered.connect(self._onOpenMask)
        self._ui._mnuFileQuit.triggered.connect(self._onQuit)
        self._ui._mnuFileSave.triggered.connect(self._onSave)
        self._ui._mnuPhaseUnwrapping.triggered.connect(self._onPhaseUnwrapping)
        self._ui._mnuPhaseDemodulation.triggered\
            .connect(self._onPhaseDemodulation)

        self._state.stateChanged.connect(self._onStateChanged)

    def start(self):
        self.connectSignals()
        self.setActionConstrains()
        self._state.setState(Constrains.init_state)
        self.show()


    @pyqtSlot(set)
    def _onStateChanged(self, state):
        print "Changing state"
        for atribute in dir(self._ui):
            if hasattr(getattr(self._ui, atribute), 'constrainedTo'):
                obj = getattr(self._ui, atribute)
                if len(obj.constrainedTo & state)>0:
                    obj.setEnabled(True)
                else:
                    obj.setEnabled(False)

    def setActionConstrains(self):
        self._ui._mnuFileClose.constrainedTo = set([Constrains.data_loaded, 
                                                    Constrains.file_open])
        self._ui._mnuFileOpen.constrainedTo = self._state.All() - \
            set([self._state.busy_state,\
                     self._state.data_processing])
        self._ui._mnuFileOpenMask.constrainedTo = set([Constrains.data_loaded,
                                                       Constrains.file_open])
        self._ui._mnuFileQuit.constrainedTo = self._state.All()
        self._ui._mnuFileSave.constrainedTo = set([Constrains.data_loaded,
                                                   Constrains.file_open])
        self._ui._mnuPhaseUnwrapping.constrainedTo = \
            set([Constrains.data_loaded])
        self._ui._mnuPhaseDemodulation.constrainedTo = \
            set([Constrains.data_loaded])

    @pyqtSlot()
    def _onClose(self):
        pass

    @pyqtSlot()
    def _onOpen(self):
        dlg = FileDialog(self, "Open file")
        dlg.setModal(True)
        dlg.exec_()

    @pyqtSlot()
    def _onOpenMask(self):
        pass

    @pyqtSlot()
    def _onQuit(self):
        pass

    @pyqtSlot()
    def _onSave(self):
        pass

    @pyqtSlot()
    def _onPhaseUnwrapping(self):
        pass

    @pyqtSlot()
    def _onPhaseDemodulation(self):
        pass
