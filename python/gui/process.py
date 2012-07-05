from PyQt4.QtCore import QThread, QMutex, QReadWriteLock, \
                         pyqtSignal, pyqtSlot, QMutexLocker, \
                         QReadLocker, QWriteLocker
from copy import copy

class Process(QThread):
    _mutex = QMutex()
    _lock = QReadWriteLock()
    _input = None
    _output = None
    _parameters = None
    state = 0
    
    progress = pyqtSignal(object)
    def __init__(self, parent = None):
        super(Process, self).__init__(parent)
        
    def beginRead(self):
        self._lock.lockForRead()
    
    def endRead(self):
        self._lock.unlock()
        
    def setInput(self, inp):
        with QMutexLocker(self._mutex):
            self._input = copy(inp)
                
    def setParameters(self, parameters):
        with QMutexLocker(self._mutex):
            self._parameters = copy(parameters)
        
    def getInput(self):
        with QMutexLocker(self._mutex):
            return copy(self._input)
    
    def getOutput(self):
        with QMutexLocker(self._mutex):
            return copy(self._output)