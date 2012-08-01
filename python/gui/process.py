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