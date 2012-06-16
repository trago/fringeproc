# -*- coding: utf-8 -*-
from ui_mainwin import Ui_UnwrapGUI
from unwrapimage import UnwrapImage
from PySide import QtCore, QtGui
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
      pass
    
  def _openImage(self, fname):
    image = cv2.imread(fname,0)
    if(image.size!=0):
      qimage = UnwrapImage(image)
      self._image = qimage
    else:
      self._image = None
      
  def _onOpenMask(self):
    pass
  
  def _onClose(self):
    pass
  
  def _onSave(self):
    pass
  
  def _onQuit(self):
    self.close()