# -*- coding: utf-8 -*-
from ui_mainwin import Ui_UnwrapGUI
from PySide import QtCore, QtGui

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
    
    
  def _onOpen(self):
    pass
  
  def _onOpenMask(self):
    pass
  
  def _onClose(self):
    pass
  
  def _onSave(self):
    pass
  
  def _onQuit(self):
    self.close()