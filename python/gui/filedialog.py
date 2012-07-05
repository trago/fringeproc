import cv2
import os
import numpy as np
from PyQt4.QtGui import QFileDialog, QVBoxLayout
from PyQt4.QtCore import Qt
from actiondialog import ActionDialog
from process import Process, QMutexLocker, QReadLocker

class OpenFile(Process):
    file_error = 0
    file_ok = 1
    file_no_name = -1
    
    def __init__(self, parent=None):
        super(OpenFile, self).__init__(parent)
        
    def run(self):
        with QMutexLocker(self._mutex):
            self.openFile()
    
    def openFile(self):
        fname = self._input
        if(fname!=''):
            ftype = os.path.splitext(fname)[1]
            try:
                if ftype == '.flt':
                    self._output=self._openFlt(fname)
                else:
                    self._output=self._openImage(fname)
            except IOError:
                self.state = self.file_error            
        else:
            self.state = self.file_no_name
            return
        self.state = self.file_ok
        
    def _openFlt(self, fname):
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
        image = np.loadtxt(fname)

        if(image !=None):
            M,N=(int(image[0]), int(image[1]))
            image = image[2:image.shape[0]]
            image = image.reshape((M,N))
        else:
            raise IOError, "Image file can not be opened"

        return image

    def _openImage(self, fname):
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
        image = cv2.imread(fname,0)

        if(image != None):
            return image
        else:
            raise IOError, "Image file can not be opened"    


class OpenDialog(ActionDialog):
    def __init__(self, parent=None, caption = ""):
        super(OpenDialog, self).__init__(parent)
        
        self._filedlg = QFileDialog(None, Qt.Widget)
        self._filedlg.setFileMode(QFileDialog.ExistingFile)
        nameFilter = "Images (*.png *.tif *.tiff *.jpg *.jpeg *.png *.bmp)" +\
            ";;Float images (*.flt)"
        self._filedlg.setNameFilter(nameFilter)
        layout = QVBoxLayout()
        layout.setSpacing(0)
        layout.setContentsMargins(0,0,0,0)
        layout.addWidget(self._filedlg)
        
        self.setLayout(layout)
        self.setWindowTitle(caption)
        
        self._filedlg.finished.connect(self.fileDlgDone)

    def fileDlgDone(self, result):
        self._process = OpenFile()
        if result == 1:
            files = self._filedlg.selectedFiles()
            self._process.setInput(files[0])
        self.done(result)