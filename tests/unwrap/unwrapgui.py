# -*- coding: utf-8 -*-
"""
Main graphic user interface of fringeproc application.

.. :moduleauthor: Julio C. Estrada <julio@cio.mx>
"""
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import Qt, QObject, pyqtSlot, pyqtSignal
from ui_mainwin import Ui_UnwrapGUI
from unwrapimage import UnwrapImage
from unwrappixmapitem import UnwrapPixmapItem
from dlgunwrap import DlgUnwrap
from readimage import Image
import cv2
import os

class Constrains(QObject):
    """
    Manages constrains for the actions presented in the user interface.

    There is a set of constrains and a set of current constrains that here
    we refer as state. We can set the constrains as a single constrain or
    by passing a lis of constrains. If we set a constrain that is not registered
    here, a `KeyError` exception is raised.

       :attribute init_state: Initial state.
       :attribute data_loaded: The data has been loaded.
       :attribute data_unloaded: The data has been unloaded.
       :attribute data_saved: The data has been saved.
       :attribute data_processing: The data is being processed.
       :attribute data_processed: The data has been processed.
       :attribute data_invalid: The data is invalid.
       :attribute action_canceled: The action was canceled.
       :attribute action_executed: The action was excetude.
       :attribute action_accepted: The action was accepted.
       :attribute file_open: The file is open.
       :attribute file_closed: The file is close.
       :attribute file_saved: The file is saved.
       :attribute user_interacting: The user is interaction.
       :attribute busy_state: The application is busy.

       :attribute _constrain: The set of registered constrains

       :author: Julio C. Estrada <julio@cio.mx>
    """

    init_state = 0 # Initial state
    data_loaded = 1 # The data has been loaded
    data_unloaded = 2 # The data has been unloaded
    data_saved = 3 # The data has been saved
    data_processing = 4 # The data is being processed
    data_processed = 5 # The data has been processed
    data_invalid = 6 # The data is invalid

    action_canceled = 7 # The action was canceled
    action_executed = 8 # The action was excetude
    action_accepted = 9 # The action was accepted

    file_open = 10 # The file is open
    file_closed = 11 # The file is close
    file_saved = 12 # The file is saved

    user_interacting = 13 # The user is interaction

    busy_state = 14 # The application is busy

    # The set of constrains.
    _constrains = set([init_state, data_loaded, data_saved, data_unloaded,
                       data_processing, data_processed, data_processed,
                       action_accepted, action_canceled, action_executed,
                       file_closed, file_open, file_saved,
                       user_interacting, busy_state])
    # The current state
    _state = set([init_state])

    # Signal to notify that the state has been changed
    stateChanged = pyqtSignal(int)

    def __init__(self, parent=None):
        """
        Initializes the QObject.

           :author: Julio C. Estrada <julio@cio.mx>
        """
        super(Constrains, self).__init__(parent)


    def setState(self, uistate):
        """
        Sets the current state.

        The current state can be set as a single constrain or a list of constrains.

           :param uistate: The constrain or constrains to be set in the current state.
           :type uistate: int or str.
           :raises:
              - KeyError if the constrains given are not registered in `Constrains`
              - TypeError if is not passed an integer or a list of integers

        :author: Julio C. Estrada <julio@cio.mx>
        """
        if isinstance(uistate, list):
            state = set(uistate)
        elif isinstance(uistate, int):
            state = set([uistate])
        else:
            raise TypeError, "Argument must be int or list"

        if len(state & self._constrains) > 0:
            self._state = state
        else:
            raise KeyError, "Attemptinng to ser an unknown state"

        self.stateChanged.emit(self._constrains)

    def getState(self):
        """
        Gives the current state.

          :returns: int or list, the constrain or list of constrains
                    of the current state.
          :author: Julio C. Estrada <julio@cio.mx>
        """
        return self._state

    def All(self):
        """
        Gives the set of all registered constrains.

        :returns: set, the set of all constrains.
        :author: Julio C. Estrada <julio@cio.mx>
        """
        return Constrains._constrains.copy()

class UnwrapGUI(QtGui.QMainWindow, Ui_UnwrapGUI):
    """ Graphic user interface for fringeproc.

    This user interface offers menus to interact with the user. The user
    interface has actions an presents data to the user.

    Logical design
    ==============
    This application is a tool for fringe pattern analysis and signal processing.
    Then, the main data that this application uses is *image data*. The user 
    interface has an area where the *image data* is shown to the user as an image.
    This area is a `QtGui.QGraphicsView`. The image data is wrapped in a
    `unwrappixmapitem.UnwrapPixmapItem` which is a `QtGui.QPixmapItem` and is
    presented in the scene of the `QtGui.QGraphicsView`.

     Actions
     -------
     In this context, an action is any object in the user interface that executes
     a command. For example: the *File* menu may has the following actions:
       - *Open*
       - *Save*
       - *Close*
       - *Quit*

     Each of these is an action. Each action has attached a constrain that enables
     or disables this action only if one of its constrains matches the current 
     state of constrains. Then, to manage this, each user action must set the 
     current constrain state to one of the defined in `Constrains`.

     Actions that process data are called systems. Each system has its dialog for
     the user. In the system's dialog the user sets the paremeters of the system
     being executed. This user interface mantains a reference to the current
     system's dialog and implements methods and operations that responds to the
     system's dialog. Also, the user interface mantains a reference to the
     constrains to mantain the current state of constrains set by the user action.

      :attribute _image: A reference to the `unwrappixmapitem.UnwrapPixmapItem` that 
                         wraps the image data and tha is show in the application 
                         *graphics view*.
      :type _image: unwrappixmapitem.UnwrapPixmapItem
      :attribute _scene: The graphics scene that shows the graphic elements (*image 
                         data*) graphics view of the user interface.
      :type _scene: QtGui.QGraphicsScene
      :attribute _system_dlg: Instance of the current system dialog.
      :type _system_dlg: classes extended from QtGui.QDialog

    Author: Julio C. Estrada <julio@cio.mx>
    """
    # PixmapItem having the image data
    _image = object
    # The graphics scene used to show the image
    _scene = None
    # The current istance of the system's dialog
    _system_dlg = None
    # Mantains the interface state
    _actionState = Constrains()
    # A reference to the current process
    _process = None

    def __init__(self, parent=None):
        """
        Constructs the graphic user interface.

        Initializes the scene and the graphics view.

         :param parent: A widge parent. If it is `None` it means that it is the
                        principal widget.
         :type parent: QtGui.QWidget

         :author: Julio C. Estrada <julio@cio.mx>
        """
        super(UnwrapGUI,self).__init__(parent)
        self.setupUi(self)
        self._connectActions()

        self._actionState.setState(Constrains.init_state)

        self._scene = QtGui.QGraphicsScene()
        self._graphicsView.setScene(self._scene)

    def _connectActions(self):
        """
        Connects the signals and slots of menu actions and defines its constrains.

          :author: Julio C. Estrada <julio@cio.mx>
        """
        self._mnuFileClose.triggered.connect(self._onClose)
        self._mnuFileOpen.triggered.connect(self._onOpen)
        self._mnuFileOpenMask.triggered.connect(self._onOpenMask)
        self._mnuFileQuit.triggered.connect(self._onQuit)
        self._mnuFileSave.triggered.connect(self._onSave)
        self._mnuPhaseUnwrapping.triggered.connect(self._onPhaseUnwrapping)
        self._mnuPhaseDemodulation.triggered.connect(self._onPhaseDemodulation)

        self._mnuFileClose.constrainedTo = set([Constrains.data_loaded, 
                                                Constrains.file_open])
        self._mnuFileOpen.constrainedTo = self._actionState.All() - \
            set([self._actionState.busy_state])
        self._mnuFileOpenMask.constrainedTo = set([Constrains.data_loaded,
                                                   Constrains.file_open])
        self._mnuFileQuit.constrainedTo = self._actionState.All()
        self._mnuFileSave.constrainedTo = set([Constrains.data_loaded,
                                               Constrains.file_open])
        self._mnuPhaseUnwrapping.constrainedTo = set([Constrains.data_loaded])
        self._mnuPhaseDemodulation.constrainedTo = set([Constrains.data_loaded])


        self._actionState.stateChanged.connect(self._onUpdateInterface)

    @pyqtSlot()  
    def _onPhaseUnwrapping(self):
        """
        Slot called when the user selects 'Phase unwrapping' from the system menu.

        While is being executed, this sets the following states:
          - `Constrains.busy_state`
          - `Constrains.data_processing`
          - `Constrains.action_executed`
        On canseled state is
          - `Constrains.action_canceled`
        Author: Julio C. Estrada <julio@cio.mx>
        """

        if self._image is not None:
            self._system_dlg = DlgUnwrap(self)
            dlg = self._system_dlg

            self._actionState.setState([Constrains.busy_state,
                                        Constrains.data_processing,
                                        Constrains.action_executed])
            dlg.exec_()
            self._actionState.setState(Constrains.action_canceled)

    def _onPhaseDemodulation(self):
        """
        _onPhaseDemodulation()
        Called when the user selects 'Phase demodulation' from the system menu.

        Author: Julio C. Estrada <julio@cio.mx>
        """
        self._actionState.setState(Constrains.data_processed)

    @pyqtSlot()
    def _onOpen(self):
        """
        _onOpen()
        Called when the user selects 'Open' from file menu.

        It launches a dialog for the user in order to select the file to open.
        When the file is opened it is created a QPixmapItem that holds the image
        data as a numpy array.

        Author: Julio C. Estrada <julio@cio.mx>
        """
        fileFilters = ["Image files (*.png *.jpg *.tif *.bmp)",
                       "Flt files (*.flt)"]
        fname = QtGui.QFileDialog.getOpenFileName(self, "Open image data", 
                                                  QtCore.QDir.currentPath(),
                                                  fileFilters[0]+';;'+fileFilters[1])
        self.setCursor(Qt.BusyCursor)
        self._process = Image()
        self._process.finished.connect(self._onProcessOpen)
        self._process.openFile(fname)
        self.statusBar().showMessage("Loading image file!!!")

        self._actionState.setState(Constrains.busy_state)

    @pyqtSlot(int)
    def _onProcessOpen(self, state):
        if self._process.getState() == self._process.image_loaded:
            for item in self._scene.items():
                self._scene.removeItem(item)
            self._image = UnwrapPixmapItem(self._process.getData())
            self._scene.addItem(self._image)
            self._image.setMoveEventHandler(self._onImageCursorOver)
            self._graphicsView.setSceneRect(self._scene.itemsBoundingRect())

            self._process.finished.disconnect(self._onProcessOpen)
            self.setCursor(Qt.ArrowCursor)
            self.statusBar().showMessage("")

            self._actionState.setState([Constrains.file_open, Constrains.data_loaded])

    @pyqtSlot()
    def _onOpenMask(self):
        """ _onOpenMask()
        Called when user selects 'Open mask' from file menu in order to apply this
        mask to an already loaded image data.

        Author: Julio C. Estrada <julio@cio.mx>
        """
        fileFilters = "Image files (*.png *.jpg *.tif *.bmp)"
        fname = QtGui.QFileDialog.getOpenFileName(self, "Open mask image", 
                                                  QtCore.QDir.currentPath(),
                                                  fileFilters)
        if(fname!='' and self._image!=None):
            image = self._image.getImage().getData('reference')
            self._openImage(fname, 'keep')
            if self._image != None:
                mask = self._image.getImage().getData('reference')/255
                data = image*mask
                self._image.setImage(data)
                self._actionState.setState(Constrains.data_loaded)

    def _onClose(self):
        self._actionState.setState([Constrains.file_closed, 
                                    Constrains.data_unloaded])

    def _onSave(self):
        fileFilters = "Image files (*.png *.jpg *.tif)"
        fname = QtGui.QFileDialog.getSaveFileName(self, "Save data", 
                                                  QtCore.QDir.currentPath(),
                                                  fileFilters)
        if(fname[0]!=''):
            #self._image.save(fname[0])
            self._actionState.setState([Constrains.data_loaded, 
                                        Constrains.file_saved])


    def _onQuit(self):
        self.close()

    def _onImageCursorOver(self, pos):
        """_onImageCursosOver(pos)
        Called when the cursor pases over the image.

        Parameters:
         * pos is the (x,y) position
           :type: QPointF

        Notes:
         * This is not an slot, this is a callback function.

        Author: Julio C. Estrada
        """
        x = int(pos.x())
        y = int(pos.y())
        data = self._image.getImage().getData('reference')
        try:
            text = "(" + str(x) + ", " + str(y) + ") = " + \
                str(data[y,x])
        except IndexError:
            text = "Out of range!"
        stBar = self.statusBar()
        stBar.showMessage(text)

    def getScene(self):
        """ getScene()
        Returns the graphics scene of the user interface.

        The graphics scene is where al data to be processed is shown in a graphics
        view. Use this graphics scene to implement user interactions when needed.

        Return:
         * scene: The graphics scene
           :type: QGraphicsScene
        """
        return self._scene

    @pyqtSlot(int)
    def _onUpdateInterface(self, state):
        print "Update interface with state " + str(self._actionState.getState())

        for atribute in dir(self):
            if hasattr(getattr(self, atribute), 'constrainedTo'):
                self._constrainsOk(getattr(self, atribute))

    def _constrainsOk(self, obj):
        state = self._actionState.getState()
        if len(obj.constrainedTo & state)>0:
            obj.setEnabled(True)
            return
        obj.setEnabled(False)