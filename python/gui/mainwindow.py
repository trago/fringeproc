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
"""
mainwindow module
-----------------

:platform: Unix, MacOS X
:sinopsys: Main window.

.. moduleauthor:: Julio C. Estrada <julio@cio.mx>

The main window of the graphic user interface.

The graphic user interface is composed by the following:

- **The menu.**
  It get acces to different commands.
- **The view area.**
  Here it is shown the images that are processed and results.
"""
from PyQt4.QtGui import QMainWindow, QGraphicsScene
from PyQt4.QtCore import pyqtSlot
from ui_mainwin import Ui_MainGUI
from constrains import Constrains
from filedialog import OpenDialog, SaveDialog
from image import Image
from pixmapitem import PixmapItem
from document import Document

class MainWindow(QMainWindow):
    """
    This is the main window of the user interface.
    
    The user interface interacts with the user. The main window has menus that
    let user do actions or execute commands. Actions are enabled when the
    conditions for executing an action are present. For example, you can not
    apply some kind of process if there is not any data present. This behaivor
    is implemented by using some kind of a state machine.
    
    The state machine works in the folliwing way: The aplication has states
    are constrain that enable or disable some actions from the user interface.
    When an action is executed, it stablishs the state that this action
    produces. For example, when user execute an action like *open file*, when
    the file is opened and its conted loaded the action will produce a 
    *data loaded* state. This state is a constrains that will enable or
    disable other actions.
    
    An action is a command to be executed by the user. Then, most actions has
    a dialog that interacts with the user to set parameters for the command
    being executed.
    
    User loads data for being processed. When the user loads data, this data is
    treated as a document. Then, a document represents the data on which the
    user is working on.
    
    **Protected attributes**
    
    .. attribute:: _state
    
       type: Constrains
       
       Has the current state of the application.
    .. attribute:: _actionDialog
    
       type: ActionDialog
       
       Reference to the current action dialog executed.
    .. attribute:: _doc
    
       type: Document
       
       Reference to the current document
    """
    _state = None
    _actionDialog = None
    _doc = None
    def __init__(self, parent=None):
        """
        Constructs the main window.
        
        :param parent: The parent window of this main window.
           
           Usually you pass `parent=None` in most cases.
        """
        super(MainWindow, self).__init__(parent)
        self._ui = Ui_MainGUI()
        self._ui.setupUi(self)
        scene = QGraphicsScene()
        self._ui._graphicsView.setScene(scene)
        
        self._state = Constrains()

    def connectSignals(self):
        """
        Connects the signals and slots of user actions.
        
        User actions are accesed usually from a graphic menu. When user selects
        an action, this actions emits a signal that is catched by an slot.
        Then, an slot is a function that executes the action.

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
        """
        Starts the application is main window.
        
        When the main window is started, action signals are connected, the
        constrains of each action is set, the initial state is set and
        the main window is shown on the cumputer display.
        
        :author: Julio C. Estrada <julio@cio.mx>
        """
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
        dlg = SaveDialog(self, "Save file")
        dlg.setInput(self._doc.getImage())
        dlg.setModal(True)
        dlg.taskFinished.connect(self._onOpenFinished)
        dlg.show()
        self._actionDialog = dlg
        
        self._state.setState(Constrains.user_interacting)

    @pyqtSlot(int)
    def _onSaveFinished(self, res):
        if res == 1:
            self.statusBar().showMessage("Image saved", 3000)
        else:
            self.statusBar().showMessage("Image not saved", 3000)
        self._state.setState([Constrains.data_saved,
                              Constrains.data_loaded])

    @pyqtSlot()
    def _onPhaseUnwrapping(self):
        pass

    @pyqtSlot()
    def _onPhaseDemodulation(self):
        pass
