from PyQt4.QtCore import *
from PyQt4.QtGui import *
from ui_maingui import Ui_MainWindow
from imviewer import ImViewer

import cv2

class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self, parent =None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        
        self.connectEvents()
        
    def connectEvents(self):
        self.connect(self.actionFileOpen, SIGNAL('activated()'), self.show_plot)

    def show_plot(self):
        viewer = ImViewer()
        im = cv2.imread('/home/julio/workspace/trabajo/ocr-placas/c++/tests/tk14k00 (13).png')
        
        self.desktopLayout.addWidget(viewer)
        viewer.setData(im)
        viewer.show()
        
