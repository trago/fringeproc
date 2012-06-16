import sys
from PySide import QtCore
from PySide import QtGui
from unwrapgui import UnwrapGUI

def main(argv):
  app = QtGui.QApplication(argv)
  gui = UnwrapGUI()
  gui.show()
  
  return app.exec_()


if __name__ == "__main__":
  main(sys.argv)


