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

from PyQt4.QtGui import QMainWindow, QGraphicsScene
from PyQt4.QtCore import pyqtSlot
from ui_mainwin import Ui_MainGUI
from constrains import Constrains
from filedialog import OpenDialog
from image import Image
from pixmapitem import PixmapItem
from document import Document

class MainWindow(QMainWindow):
    _state = None
    _actionDialog = None
    _doc = None
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self._ui = Ui_MainGUI()
        self._ui.setupUi(self)
        scene = QGraphicsScene()
        self._ui._graphicsView.setScene(scene)
        
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
        dlg = OpenDialog(self, "Open file")
        dlg.setModal(True)
        dlg.taskFinished.connect(self._onOpenFinished)
        dlg.show()
        self._actionDialog = dlg
        
        self._state.setState(Constrains.user_interacting)
        
    @pyqtSlot(int)
    def _onOpenFinished(self, res):
        if res == 1:
            self._doc = Document(self._actionDialog.getOutput())
            self.showImage(self._doc.getImage())
            self._state.setState(Constrains.data_loaded)
            
        scene = self._ui._graphicsView.scene()
        self._ui._graphicsView.setSceneRect(scene.itemsBoundingRect())

    def showImage(self, image):
        scene = self._ui._graphicsView.scene()
        items = scene.items()
        for item in items:
            scene.removeItem(item)
            
        item = PixmapItem(image)
        item.setMoveEventHandler(self._onMouseHoverImage)
        scene.addItem(item)
        
    def _onMouseHoverImage(self, pos, image):
        strPos = "pixel(%d, %d) = " % (pos.x(),pos.y())
        strPos += str(image[pos.y(), pos.x()])
        self.statusBar().showMessage(strPos, 3000)

    @pyqtSlot()
    def _onOpenMask(self):
        pass

    @pyqtSlot()
    def _onQuit(self):
        self.close()

    @pyqtSlot()
    def _onSave(self):
        pass

    @pyqtSlot()
    def _onPhaseUnwrapping(self):
        pass

    @pyqtSlot()
    def _onPhaseDemodulation(self):
        pass
