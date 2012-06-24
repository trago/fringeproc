from PyQt4.QtCore import QThread, QMutex,\
     QMutexLocker, QReadLocker, pyqtSignal

class Process(QThread):
    _mutex = QMutex()
    _lock = None

    process_stopped = 0
    process_finished = 1
    process_running = 2

    _state = process_finished

    finished = pyqtSignal(int)

    def __init__(self, parent=None):
        super(Process, self).__init__(parent)

    def stop(self):
        with QMutexLocker(self._mutex):
            _state = stopped

    def getState(self):
        with QMutexLocker(self._mutex):
            return self._state

    def emitFinished(self):
        self.finished.emit(self._state)

    def run(self):
        with QMutexLocker(self._mutex):
            _state = self.process_finished;
        self.emitFinished()