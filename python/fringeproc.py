import sip
sip.setapi("QtString",2)
import matplotlib
matplotlib.use('QtAgg')

from PyQt4.QtGui import QApplication
import sys

from gui.maingui import MainWindow


def main():
    app = QApplication(sys.argv)
    mainwin = MainWindow()
    mainwin.show()
    
    sys.exit(app.exec_())
    
    
if __name__=='__main__':
    main()