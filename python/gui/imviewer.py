from __future__ import division
from PyQt4 import QtGui, QtCore

import numpy as np
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

class ImViewer(FigureCanvas):
    def __init__(self, parent=None, width=2, height=1.5, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        self.axes.hold(False)
        
        super(ImViewer, self).__init__(fig)
        self.setParent(parent)
        super(ImViewer, self).setSizePolicy(QtGui.QSizePolicy.Expanding,
                                            QtGui.QSizePolicy.Expanding)
        super(ImViewer, self).updateGeometry()
                
    def setData(self, img, color='default'):
        self.axes.imshow(img)
        self.axes.set_axis_off()
        