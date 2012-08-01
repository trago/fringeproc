# Copyright (c) 2012, Julio C. Estrada
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:

# + Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.

# + Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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