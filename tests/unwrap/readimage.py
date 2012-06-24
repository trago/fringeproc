import cv2
import os
import numpy as np
import fringeprocess as fringep

class Image(fringep.Process):
    image_loaded = 100
    image_notloaded = 101

    _fname = None
    _format = None
    _data = None

    def __init__(self, parent=None):
        super(Image, self).__init__(parent)

    def openFile(self, fname):
        self._fname = fname
        if(fname!=''):
            ftype = os.path.splitext(fname)[1]
            self._format = ftype
            self.start()
        else:
            raise IOError, "No file name given"

    def getData(self):
        return self._data

    def run(self):
        with fringep.QMutexLocker(self._mutex):
            try:
                if self._format == '.flt':
                    self._data=self._openFlt(self._fname)
                else:
                    self._data=self._openImage(self._fname)
                self._state = self.image_loaded
            except IOError:
                self._state = self.image_notloaded
        self.emitFinished()

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