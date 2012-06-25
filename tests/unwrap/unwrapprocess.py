from dlgprogressresult import ProgressResult
import fringeprocess as fringep
import fringeproc as fringes

class Unwrap(fringep.Process):
    _progress = None # The ProgressResult Dialog
    _args = None # The arguments for the phase unwrapping system
    _output = None # The data output of the system
    
    unwrap_canceled = fringep.Process.process_stopped
    unwrap_finished = 100
    unwrap_on = 102
    
    def __init__(self,  progress, parent=None):
        super(Unwrap, self).__init__(parent)
        self._args = parent.getParameters()
        self._progress = progress
        self._state = self.unwrap_on
        
        self.progress.connect(self.onShowProgress)
        self.finished.connect(self._progress.enableAccept)
        
        self._progress.finished.connect(self.onFinished)
        
    def onShowProgress(self, data):
        self._progress.showImage(data)
        
    def onFinished(self, result):
        if result == 1:
            self.finished.emit(self._state)
        else:
            self.finished.emit(self.unwrap_canceled)
        
    def showProgress(self):
        self._progress.setModal(True)
        self._progress.show()
        
    def run(self):
        self._args = self.parent().getParameters()
        system = fringes.Unwrap(self._args['data'],
                                self._args['tau'],
                                self._args['smooth'],
                                self._args['nSize'])
        system.setPixel(self._args['pixel'])
        
        with fringep.QReadLocker(self._rwLock):
            state = self._state
            
        with fringep.QMutexLocker(self._mutex):
            next_iter = system.runInteractive(500)
                
        while next_iter and state!=self.unwrap_canceled:
            data = system.getOutput()
            self.progress.emit(data)
            with fringep.QMutexLocker(self._mutex):
                next_iter = system.runInteractive(500)
            
        self.progress.emit(data)
        self._output = system.getOutput()
        if state == self.unwrap_on:
            with fringep.QWriteLocker(self._rwLock):
                self._state = self.unwrap_finished
            self.finished.emit(self.unwrap_finished)
        else:
            self.finished.emit(self.unwrap_canceled)
        