from PyQt4.QtCore import QThread, QMutex, QReadWriteLock, \
                         pyqtSignal, pyqtSlot

class Process(QThread):
    _mutex = QMutex()
    _lock = QReadWriteLock()
    _input = None
    _output = None
    _parameters = None
    
    progress = pyqtSignal(object)
    status = 0
    def __init__(self, parent = None):
        super(Process, self).__init__(parent)
        
    def beginRead(self):
        self._lock.lockForRead()
    
    def endRead(self):
        self._lock.unlock()
        
    def setInput(self, inp):
        self._input = inp
        
    def setOutput(self, out):
        self._output = out
        
    def setParameters(self, parameters):
        self._parameters = parameters
        
    def getInput(self):
        return self._input
    
    def getOutput(self):
        return self._output