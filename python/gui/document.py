import numpy as np

class Document(object):
    def __init__(self, image=None):
        self._image = image
        self._mask = np.ones(image.shape, dtype=np.uint8)
        
    def setImage(self, image):
        self._image=image
    
    def setMask(self, mask):
        self._mask = mask
        
    def getImage(self):
        return self._image
    
    def getMask(self):
        return self._mask