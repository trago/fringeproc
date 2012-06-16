# -*- coding: utf-8 -*-
from ui_mainwin import Ui_UnwrapGUI
from unwrapimage import UnwrapImage
from unwrappixmapitem import UnwrapPixmapItem
from PySide import QtCore, QtGui
from PySide.QtCore import Qt
import numpy as np
import cv2

class UnwrapGUI(QtGui.QMainWindow, Ui_UnwrapGUI):
  def __init__(self, parent=None):
    super(UnwrapGUI,self).__init__(parent)
    self.setupUi(self)
    
    self._scene = QtGui.QGraphicsScene()
    self.graphicsView.setScene(self._scene)
    
    self.mnuFileClose.triggered.connect(self._onClose)
    self.mnuFileOpen.triggered.connect(self._onOpen)
    self.mnuFileOpenMask.triggered.connect(self._onOpenMask)
    self.mnuFileQuit.triggered.connect(self._onQuit)
    self.mnuFileSave.triggered.connect(self._onSave)
    
    self._image = None # Los datos de la imagen que se abrio
    
    
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
          self.graphicsView.setScene(self._scene)
        else:
          pitem = UnwrapPixmapItem(QtGui.QPixmap.fromImage(self._image))
          pitem.setMoveEventHandler(self._onImageCursorOver)
          self._scene.addItem(pitem)

    
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
    fname = QtGui.QFileDialog.getOpenFileName(self, "Open image data", 
                                              QtCore.QDir.currentPath(),
                                              fileFilters)
    if(fname[0]!=''):
      image = self._image
      self._openImage(fname[0])
      if self._image != None:
        mask = self._image.mdata/255
        data = image.mdata*mask
        self._image = UnwrapImage(data)
        if len(self._scene.items())!=0:
          self._scene = QtGui.QGraphicsScene()
          pitem = UnwrapPixmapItem(QtGui.QPixmap.fromImage(self._image))
          pitem.setMoveEventHandler(self._onImageCursorOver)
          self._scene.addItem(pitem)
          self.graphicsView.setScene(self._scene)
        else:
          pitem = UnwrapPixmapItem(QtGui.QPixmap.fromImage(self._image))
          pitem.setMoveEventHandler(self._onImageCursorOver)
          self._scene.addItem(pitem)
  
  def _onClose(self):
    pass
  
  def _onSave(self):
    pass
  
  def _onQuit(self):
    self.close()
    
  def _onImageCursorOver(self, pos):
    x = int(pos.x())
    y = int(pos.y())
    text = "(" + str(x) + ", " + str(y) + ") = " + str(self._image.mdata[y,x])
    stBar = self.statusBar()
    stBar.showMessage(text)
    