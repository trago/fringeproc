# -*- coding: utf-8 -*-
"""
Main graphic user interface of fringeproc application.

Author: Julio C. Estrada
"""
from ui_mainwin import Ui_UnwrapGUI
from unwrapimage import UnwrapImage
from unwrappixmapitem import UnwrapPixmapItem
from dlgunwrap import DlgUnwrap
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import Qt
import fringeproc as fringes
import numpy as np
import cv2
import os

class UnwrapGUI(QtGui.QMainWindow, Ui_UnwrapGUI):
  """ Graphic user interface for fringeproc.
  
  Properties:
   * _image: A reference to the PixmapItem that contains the image data
     being shown in the application scene.
   * _scene: The graphics scene that shows the graphic elements in the user
     interface.
     
  Author: Julio C. Estrada <julio@cio.mx>
  """
  # PixmapItem having the image data
  _image = object
  # The graphics scene used to show the image
  _scene = None
  
  def __init__(self, parent=None):
    """ UnwrapGUI(parent=None)
    Constructs the graphic user interface.
    
    Initializes the scene and the graphics view.
    
    Parameters:
     * parent: A widge parent. If it is `None` it means that it is the
       principal widget.
       :type: QWidget
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    super(UnwrapGUI,self).__init__(parent)
    self.setupUi(self)
    self._connectActions()
    
    self._scene = QtGui.QGraphicsScene()
    self._graphicsView.setScene(self._scene)
    
  def _connectActions(self):
    """
    _connectActions()
    Connects the signals and slots of menu actions.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    self._mnuFileClose.triggered.connect(self._onClose)
    self._mnuFileOpen.triggered.connect(self._onOpen)
    self._mnuFileOpenMask.triggered.connect(self._onOpenMask)
    self._mnuFileQuit.triggered.connect(self._onQuit)
    self._mnuFileSave.triggered.connect(self._onSave)
    self._mnuPhaseUnwrapping.triggered.connect(self._onPhaseUnwrapping)
    self._mnuPhaseDemodulation.triggered.connect(self._onPhaseDemodulation)
    
  def _onPhaseUnwrapping(self):
    """
    _onPhaseUnwrapping()
    Called when the user selects 'Phase unwrapping' from the system menu.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    if self._image is not None:
      dlg = DlgUnwrap(self)
      resp = dlg.exec_()
      
      if resp == 1:
        params = dlg.getParameters()
        data_input = self._image.getDataF()*2*np.pi-np.pi
        unwrap = fringes.Unwrap(data_input, params['tau'], params['smooth'],
                                params['nSize'])
        unwrap.setPixel(params['pixel'])
        print dlg.getParameters()
  
  def _onPhaseDemodulation(self):
    """
    _onPhaseDemodulation()
    Called when the user selects 'Phase demodulation' from the system menu.
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    pass
    
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
    ftype = os.path.splitext(fname)[1]
    if(fname!=''):
      if(ftype!='.flt'):
        self._openImage(fname)
      elif ftype=='.flt':
        self._openFlt(fname)
        
      if self._image != None:
        if len(self._scene.items())!=0:
          self._scene = QtGui.QGraphicsScene()
          self._graphicsView.setScene(self._scene)
        self._scene.addItem(self._image)
        self._image.setMoveEventHandler(self._onImageCursorOver)

  def _openImage(self, fname, flag='new'):
    """ _openImage(fname, flag='new')
    Opens the image file and loads its data.
    
    It opens an image file in format png, jpg, tif or bmp and loads its data
    creating the PixmapItem or setting the image data to an already created
    PixmapItem.
    
    Parameters:
     * fname: Is the fale name to be opened.
       :type: str
     * flag: if flag='new' indicates that a PixmapItem is going to be created,
       otherwise the image data is set to the current PixmapItem.
       :type: str
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    self.setCursor(Qt.BusyCursor)    
    image = cv2.imread(fname,0)
    self.setCursor(Qt.ArrowCursor)

    if(image.size!=0):
      if flag=='new':
        self._image = UnwrapPixmapItem(image)
      else:
        self._image.setImage(image)
    else:
      self._image = None
  
  def _openFlt(self, fname, flag='new'):
    """ _openFlt(fname, flag='new')
    Opens a file having floating point data.
    
    It opens a text file with extension .flt having floating point data and loads
    these into an array to bi used as image data. The format for this text file
    is one value by line and the first two lines have the number of rows
    and number of columns of the image.
    
    Parameters:
     * fname: Is the file name to be opened.
       :type: str
     * flag: if flag='new' indicates that a PixmapItem is going to be created,
       otherwise the image data is set to the current PixmapItem.
       :type: str
    
    Author: Julio C. Estrada <julio@cio.mx>
    """
    self.setCursor(Qt.BusyCursor)
    image = np.loadtxt(fname)
    self.setCursor(Qt.ArrowCursor)

    if(image !=None):
      M,N=(int(image[0]), int(image[1]))
      image = image[2:image.shape[0]]
      image = image.reshape((M,N))
      if flag=='new':
        self._image = UnwrapPixmapItem(image)
      else:
        self._image.setImage(image)
    else:
      self._image = None
      
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

  def _onClose(self):
    pass  

  def _onSave(self):
    fileFilters = "Image files (*.png *.jpg *.tif)"
    fname = QtGui.QFileDialog.getSaveFileName(self, "Save data", 
                                              QtCore.QDir.currentPath(),
                                              fileFilters)
    if(fname[0]!=''):
      self._image.save(fname[0])
  
  def _onQuit(self):
    self.close()
    
  def _onImageCursorOver(self, pos):
    x = int(pos.x())
    y = int(pos.y())
    text = "(" + str(x) + ", " + str(y) + ") = " + \
      str(self._image.getImage().getData('reference')[y,x])
    stBar = self.statusBar()
    stBar.showMessage(text)
    