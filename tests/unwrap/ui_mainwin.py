# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwin.ui'
#
# Created: Sat Jun 16 12:47:03 2012
#      by: pyside-uic 0.2.13 running on PySide 1.1.0
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_UnwrapGUI(object):
    def setupUi(self, UnwrapGUI):
        UnwrapGUI.setObjectName("UnwrapGUI")
        UnwrapGUI.resize(798, 600)
        self.centralwidget = QtGui.QWidget(UnwrapGUI)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout = QtGui.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.graphicsView = QtGui.QGraphicsView(self.centralwidget)
        self.graphicsView.setAutoFillBackground(False)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        self.graphicsView.setBackgroundBrush(brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.NoBrush)
        self.graphicsView.setForegroundBrush(brush)
        self.graphicsView.setObjectName("graphicsView")
        self.horizontalLayout.addWidget(self.graphicsView)
        UnwrapGUI.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(UnwrapGUI)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 798, 25))
        self.menubar.setObjectName("menubar")
        self.mnuFile = QtGui.QMenu(self.menubar)
        self.mnuFile.setObjectName("mnuFile")
        UnwrapGUI.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(UnwrapGUI)
        self.statusbar.setObjectName("statusbar")
        UnwrapGUI.setStatusBar(self.statusbar)
        self.mnuFileOpen = QtGui.QAction(UnwrapGUI)
        self.mnuFileOpen.setObjectName("mnuFileOpen")
        self.mnuFileSave = QtGui.QAction(UnwrapGUI)
        self.mnuFileSave.setObjectName("mnuFileSave")
        self.mnuFileQuit = QtGui.QAction(UnwrapGUI)
        self.mnuFileQuit.setObjectName("mnuFileQuit")
        self.mnuFileClose = QtGui.QAction(UnwrapGUI)
        self.mnuFileClose.setObjectName("mnuFileClose")
        self.mnuFileOpenMask = QtGui.QAction(UnwrapGUI)
        self.mnuFileOpenMask.setObjectName("mnuFileOpenMask")
        self.mnuFile.addAction(self.mnuFileOpen)
        self.mnuFile.addAction(self.mnuFileOpenMask)
        self.mnuFile.addAction(self.mnuFileSave)
        self.mnuFile.addAction(self.mnuFileClose)
        self.mnuFile.addSeparator()
        self.mnuFile.addAction(self.mnuFileQuit)
        self.menubar.addAction(self.mnuFile.menuAction())

        self.retranslateUi(UnwrapGUI)
        QtCore.QMetaObject.connectSlotsByName(UnwrapGUI)

    def retranslateUi(self, UnwrapGUI):
        UnwrapGUI.setWindowTitle(QtGui.QApplication.translate("UnwrapGUI", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.mnuFile.setTitle(QtGui.QApplication.translate("UnwrapGUI", "&File", None, QtGui.QApplication.UnicodeUTF8))
        self.mnuFileOpen.setText(QtGui.QApplication.translate("UnwrapGUI", "&Open", None, QtGui.QApplication.UnicodeUTF8))
        self.mnuFileSave.setText(QtGui.QApplication.translate("UnwrapGUI", "&Save", None, QtGui.QApplication.UnicodeUTF8))
        self.mnuFileQuit.setText(QtGui.QApplication.translate("UnwrapGUI", "&Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.mnuFileClose.setText(QtGui.QApplication.translate("UnwrapGUI", "&Close", None, QtGui.QApplication.UnicodeUTF8))
        self.mnuFileOpenMask.setText(QtGui.QApplication.translate("UnwrapGUI", "Open &mask", None, QtGui.QApplication.UnicodeUTF8))

