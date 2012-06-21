# -*- coding: utf-8 -*-
from ui_mainwin import Ui_UnwrapGUI
from unwrapimage import UnwrapImage
from unwrappixmapitem import UnwrapPixmapItem
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import Qt
import numpy as np
import cv2

class UnwrapGUI(QtGui.QMainWindow, Ui_UnwrapGUI):
  """
  Graphic user interface for fringeproc.
  """
  # Datos de la imagen que se esta trabajando
  _image = None
  # The graphics scene used to show the image
  _scene = None
  
  def __init__(self, parent=None):
    super(UnwrapGUI,self).__init__(parent)
    self.setupUi(self)
    self._connectActions()
    
    self._scene = QtGui.QGraphicsScene()
    self._graphicsView.setScene(self._scene)
    
  def _connectActions(self):
    self._mnuFileClose.triggered.connect(self._onClose)
    self._mnuFileOpen.triggered.connect(self._onOpen)
    self._mnuFileOpenMask.triggered.connect(self._onOpenMask)
    self._mnuFileQuit.triggered.connect(self._onQuit)
    self._mnuFileSave.triggered.connect(self._onSave)
    self._mnuPhaseUnwrapping.triggered.connect(self._onPhaseUnwrapping)
    self._mnuPhaseDemodulation.triggered.connect(self._onPhaseDemodulation)
    
  def _onPhaseUnwrapping(self):
    pass
  
  def _onPhaseDemodulation(self):
    pass
    
  def _onOpen(self):
    fileFilters = ["Image files (*.png *.jpg *.tif *.bmp)",
                   "Flt files (*.flt)"]
    fname = QtGui.QFileDialog.getOpenFileName(self, "Open image data", 
                                              QtCore.QDir.currentPath(),
                                              fileFilters[0]+';;'+fileFilters[1])
    if(fname[0]!=''):
      if(fname[1]==fileFilters[0]):
        self._openImage(fname[0])
      elif fname[1]==fileFilters[1]:
        self._openFlt(fname[0])
        
      if self._image != None:
        if len(self._scene.items())!=0:
          self._scene = QtGui.QGraphicsScene()
          pitem = UnwrapPixmapItem(QtGui.QPixmap.fromImage(self._image))
          pitem.setMoveEventHandler(self._onImageCursorOver)
          self._scene.addItem(pitem)
          self._graphicsView.setScene(self._scene)
        else:
          pitem = UnwrapPixmapItem(QtGui.QPixmap.fromImage(self._image))
          pitem.setMoveEventHandler(self._onImageCursorOver)
          self._scene.addItem(pitem)
        
        self._scene.show()

    
  def _openImage(self, fname):
    self.setCursor(Qt.BusyCursor)    
    image = cv2.imread(fname,0)
    self.setCursor(Qt.ArrowCursor)

    if(image.size!=0):
      qimage = UnwrapImage(image)
      self._image = qimage
    else:
      self._image = None
  
  def _openFlt(self, fname):
    self.setCursor(Qt.BusyCursor)
    image = np.loadtxt(fname)
    self.setCursor(Qt.ArrowCursor)

    if(image !=None):
      M,N=(int(image[0]), int(image[1]))
      image = image[2:image.shape[0]]
      image = image.reshape((M,N))
      
      self._image = UnwrapImage(image)
    else:
      self._image = None
      
  def _onOpenMask(self):
    fileFilters = "Image files (*.png *.jpg *.tif *.bmp)"
    fname = QtGui.QFileDialog.getOpenFileName(self, "Open mask image", 
                                              QtCore.QDir.currentPath(),
                                              fileFilters)
    if(fname[0]!=''):
      image = self._image
      self._openImage(fname[0])
      if self._image != None and image != None:
        mask = self._image.mdata/255
        data = image.mdata*mask
        self._image = UnwrapImage(data)
        if len(self._scene.items())!=0:
          self._scene = QtGui.QGraphicsScene()
          pitem = UnwrapPixmapItem(QtGui.QPixmap.fromImage(self._image))
          pitem.setMoveEventHandler(self._onImageCursorOver)
          self._scene.addItem(pitem)
          self._graphicsView.setScene(self._scene)
        else:
          pitem = UnwrapPixmapItem(QtGui.QPixmap.fromImage(self._image))
          pitem.setMoveEventHandler(self._onImageCursorOver)
          self._scene.addItem(pitem)
  
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
    text = "(" + str(x) + ", " + str(y) + ") = " + str(self._image.mdata[y,x])
    stBar = self.statusBar()
    stBar.showMessage(text)
    