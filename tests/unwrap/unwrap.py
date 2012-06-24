import sip
api2_classes = [
    'QData', 'QDateTime', 'QString', 'QTextStream',
    'QTime', 'QUrl', 'QVariant']
for cl in api2_classes:
    sip.setapi(cl, 2)

import sys
from PyQt4 import QtGui
from unwrapgui import UnwrapGUI

def main(argv):
    app = QtGui.QApplication(argv)
    gui = UnwrapGUI()
    gui.show()

    return app.exec_()


if __name__ == "__main__":
    main(sys.argv)