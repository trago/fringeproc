from PyQt4.QtGui import QApplication
from mainwindow import MainWindow

class FringeProc(object):
    """
    The FringeProc application.

    :attribute _app: Instance to the application's object.
    :type _app: `QApplication`
    :attribute _window: Instance to the main window's application.
    :type _window: `MainWindow`

    :author: Julio C. Estrada <julio@cio.mx>    
    """
    _app = None
    _window = None
    def __init__(self, argv):
        self._app = QApplication(argv)
        self._window = MainWindow()

    def start(self):
        self._window.start()
        self._app.exec_()
