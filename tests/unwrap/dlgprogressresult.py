from PyQt4.QtGui import QDialog, QGraphicsScene, QDialogButtonBox, qApp
from unwrappixmapitem import UnwrapPixmapItem
from ui_dlgresultprogress import Ui_DlgProgressResult
from fringeproc import Unwrap

class ProgressResult(QDialog):

    _ui = None
    _scene = None
    _image = None

    def __init__(self, parent=None):
        super(ProgressResult, self).__init__(parent)

        self._ui = Ui_DlgProgressResult()
        self._ui.setupUi(self)

        self._scene = QGraphicsScene()
        self._ui.graphicsView.setScene(self._scene)
        button = self._ui.btnAcceptCancel.button(QDialogButtonBox.Ok)
        button.setEnabled(False)
        
    def showImage(self, data):
        scene= self._scene
        if len(scene.items()) ==0:
            image = UnwrapPixmapItem(data)
            scene.addItem(image)
            self._image=image
        else:
            self._image.setImage(data)
        print "Mostrando imagen"
        scene.update()
        qApp.processEvents()
            
    def enableAccept(self, flag):
        button = self._ui.btnAcceptCancel.button(QDialogButtonBox.Ok)
        if flag != 0:
            button.setEnabled(True)
        else:
            button.setEnabled(False)