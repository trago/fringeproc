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

import cv2
import os
import numpy as np
from PyQt4.QtGui import QFileDialog, QVBoxLayout
from PyQt4.QtCore import Qt
from actiondialog import ActionDialog
from process import Process, QMutexLocker, QReadLocker

class OpenFile(Process):
    """
    Process to open a file.
    
    This is a process to open a data file. This data file can be an image or
    a matrix of float values. The input of this process is the name of the file
    to load. The output of this process is the data loaded from the file as
    a *numpy array*.
    """
    file_error = 0
    file_ok = 1
    file_no_name = -1
    
    def __init__(self, parent=None):
        super(OpenFile, self).__init__(parent)
        
    def run(self):
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
        
        
    def _readFltFile(self, fname):
        """
        readFltFile(fname)
        """
        iter = 0
        data = None
        try:
            with open(fname) as f:
                try:
                    M=0
                    N=0
                    i=0; j=0
                    for line in f:
                        if iter == 0:
                            M = float(line)
                        elif iter == 1:
                            N = float(line)
                        elif iter == 2:
                            data = np.zeros((M,N), dtype=float)
                        else:
                            data[i,j%N] = float(line)
                            j += 1
                            if j%N == 0:
                                i += 1
                        iter += 1
                except ValueError:
                    print 'Error: Data file %s is wrong or it does not exists!' % fname
                    exit(1)
        except IOError:
            return None
        return data
    
    
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
        image = self._readFltFile(fname)
 
        if(image == None):
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

class SaveFile(OpenFile):
    """
    Saves data in a file.
    
    This is a process that saves data in a file. The data to file is a image
    or a matrix of float values. Then the input data must be *name of the file*
    and *data to save*. The output of this process is a flag that says if the
    file has been saved succesfully or not.
    """
    def __init__(self, parent=None):
        super(SaveFile, self).__init__(parent)
        
    def run(self):
        self.saveFile()
    
    def saveFile(self):
        fname = self._input[0]
        if(fname!=''):
            ftype = os.path.splitext(fname)[1]
            try:
                if ftype == '.flt':
                    self._output=self._saveFlt(fname)
                else:
                    self._output=self._saveImage(fname)
            except IOError:
                self.state = self.file_error            
        else:
            self.state = self.file_no_name
            return
        self.state = self.file_ok
                
    def _writeFltFile(self, fname, data):
        try:
            f = open(fname,'w')
            M = str(data.shape[0])
            N = str(data.shape[1])
            f.write(M+'\n')
            f.write(N+'\n')
            for i in xrange(0,data.shape[0]):
                for j in xrange(0,data.shape[1]):
                    val = str(data[i,j])
                    f.write(val+'\n')
        except IOError:
            print 'Error: I can not write in oput file.'
            return False
        
        return True
    
    def _saveFlt(self, fname):
        data = self.getInput()[1]
        self._output = self._writeFltFile(fname, data)
        if self._output is True:
            self.state = self.file_ok
        else:
            self.state = self.file_error
    
    def _saveImage(self, fname):
        data = self.getInput()[1]
        image = cv2.normalize(data, 0, 255, cv2.NORM_MINMAX)
        self._output = cv2.imwrite(fname, image)
        
        if self._output is True:
            self.state = self.file_ok
        else:
            self.state = self.file_error

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
        
class SaveDialog(OpenDialog):
    def __init__(self, parent=None, caption = ""):
        super(SaveDialog, self).__init__(parent)
        self._filedlg.setFileMode(QFileDialog.AnyFile)
        
    def fileDlgDone(self, result):
        self._process = SaveFile()
        if result == 1:
            files = self._filedlg.selectedFiles()
            self._process.setInput((files[0], self._input))
        self.done(result)