from PyQt4.QtGui import QApplication
from mainwindow import MainWindow

class FringeProc(object):
    _app = None
    _window = None
    def __init__(self, argv):
        self._app = QApplication(argv)
        self._window = MainWindow()
        self._window.show()

    def start(self):
        self._app.exec_()
