from PyQt4.QtGui import QDialog
from PyQt4.QtCore import pyqtSignal, pyqtSlot
from copy import copy

class ActionDialog(QDialog):    
    taskFinished = pyqtSignal(int)
    def __init__(self, parent=None):
        super(ActionDialog, self).__init__(parent)
        
        self._process = None
        self._input = None
        self._output = None
        self._progressDlg = None
        
        self.setModal(True)
        self.accepted.connect(self.onAccepted)

    def getInput(self):
        return self._input
    
    def getOutput(self):
        return self._output
    
    def setInput(self, pinput):
        self._input = pinput
        
    def setOutput(self, poutput):
        self._output = poutput
        
    @pyqtSlot()
    def onAccepted(self):
        if self._process is not None:
            self._process.finished.connect(self.onProcessFinished)
            self._process.progress.connect(self.onProgressUpdate)
            self._process.start()
            self.hide()
            
            if self._progressDlg is not None:
                self._progressDlg.setModal(True)
                self._progressDlg.show()
    
    @pyqtSlot()
    def onProcessFinished(self):
        self._output = self._process.getOutput()

        self._process.finished.disconnect(self.onProcessFinished)
        self._process.progress.disconnect(self.onProgressUpdate)        
        self.taskFinished.emit(self._process.state)
        
        del self._process
        self._process = None
        if self._progressDlg is not None:
            self.progressDlg.done(1)
        
    @pyqtSlot(object)
    def onProgressUpdate(self, data):
        try:
            self._process.beginRead()
            self.updateProgress(copy(data))
        finally:
            self._process.endRead()
        
    def updateProgress(self, data):
        pass