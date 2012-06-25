from PyQt4.QtCore import QThread, QMutex,\
     QMutexLocker, QReadWriteLock, QReadLocker, QWriteLocker, pyqtSignal
from PyQt4.QtGui import QDialog

class Process(QThread):
    _mutex = QMutex()
    _rwLock = QReadWriteLock(QReadWriteLock.Recursive)

    process_stopped = 0
    process_finished = 1
    process_running = 2

    _state = process_finished

    finished = pyqtSignal(int)
    progress = pyqtSignal(object)

    def __init__(self, parent=None):
        super(Process, self).__init__(parent)

    def stop(self):
        with QMutexLocker(self._mutex):
            _state = stopped

    def getState(self):
        with QReadLocker(self._rwLock):
            return self._state

    def emitFinished(self):
        self.finished.emit(self._state)

    def run(self):
        with QMutexLocker(self._mutex):
            _state = self.process_finished;
        self.emitFinished()
        