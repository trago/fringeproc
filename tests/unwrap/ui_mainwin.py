# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwin.ui'
#
# Created: Wed Jun 20 18:56:44 2012
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_UnwrapGUI(object):
    def setupUi(self, UnwrapGUI):
        UnwrapGUI.setObjectName(_fromUtf8("UnwrapGUI"))
        UnwrapGUI.resize(798, 600)
        self.centralwidget = QtGui.QWidget(UnwrapGUI)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.horizontalLayout = QtGui.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self._graphicsView = QtGui.QGraphicsView(self.centralwidget)
        self._graphicsView.setAutoFillBackground(False)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.SolidPattern)
        self._graphicsView.setBackgroundBrush(brush)
        brush = QtGui.QBrush(QtGui.QColor(0, 0, 0))
        brush.setStyle(QtCore.Qt.NoBrush)
        self._graphicsView.setForegroundBrush(brush)
        self._graphicsView.setObjectName(_fromUtf8("_graphicsView"))
        self.horizontalLayout.addWidget(self._graphicsView)
        UnwrapGUI.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(UnwrapGUI)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 798, 25))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self._mnuFile = QtGui.QMenu(self.menubar)
        self._mnuFile.setObjectName(_fromUtf8("_mnuFile"))
        self._mnuSystems = QtGui.QMenu(self.menubar)
        self._mnuSystems.setObjectName(_fromUtf8("_mnuSystems"))
        UnwrapGUI.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(UnwrapGUI)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        UnwrapGUI.setStatusBar(self.statusbar)
        self._mnuFileOpen = QtGui.QAction(UnwrapGUI)
        self._mnuFileOpen.setObjectName(_fromUtf8("_mnuFileOpen"))
        self._mnuFileSave = QtGui.QAction(UnwrapGUI)
        self._mnuFileSave.setObjectName(_fromUtf8("_mnuFileSave"))
        self._mnuFileQuit = QtGui.QAction(UnwrapGUI)
        self._mnuFileQuit.setObjectName(_fromUtf8("_mnuFileQuit"))
        self._mnuFileClose = QtGui.QAction(UnwrapGUI)
        self._mnuFileClose.setObjectName(_fromUtf8("_mnuFileClose"))
        self._mnuFileOpenMask = QtGui.QAction(UnwrapGUI)
        self._mnuFileOpenMask.setObjectName(_fromUtf8("_mnuFileOpenMask"))
        self._mnuPhaseUnwrapping = QtGui.QAction(UnwrapGUI)
        self._mnuPhaseUnwrapping.setObjectName(_fromUtf8("_mnuPhaseUnwrapping"))
        self._mnuPhaseDemodulation = QtGui.QAction(UnwrapGUI)
        self._mnuPhaseDemodulation.setObjectName(_fromUtf8("_mnuPhaseDemodulation"))
        self._mnuFile.addAction(self._mnuFileOpen)
        self._mnuFile.addAction(self._mnuFileOpenMask)
        self._mnuFile.addAction(self._mnuFileSave)
        self._mnuFile.addAction(self._mnuFileClose)
        self._mnuFile.addSeparator()
        self._mnuFile.addAction(self._mnuFileQuit)
        self._mnuSystems.addAction(self._mnuPhaseUnwrapping)
        self._mnuSystems.addAction(self._mnuPhaseDemodulation)
        self.menubar.addAction(self._mnuFile.menuAction())
        self.menubar.addAction(self._mnuSystems.menuAction())

        self.retranslateUi(UnwrapGUI)
        QtCore.QMetaObject.connectSlotsByName(UnwrapGUI)

    def retranslateUi(self, UnwrapGUI):
        UnwrapGUI.setWindowTitle(QtGui.QApplication.translate("UnwrapGUI", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFile.setTitle(QtGui.QApplication.translate("UnwrapGUI", "&File", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuSystems.setTitle(QtGui.QApplication.translate("UnwrapGUI", "&Systems", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileOpen.setText(QtGui.QApplication.translate("UnwrapGUI", "&Open", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileSave.setText(QtGui.QApplication.translate("UnwrapGUI", "&Save", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileQuit.setText(QtGui.QApplication.translate("UnwrapGUI", "&Quit", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileClose.setText(QtGui.QApplication.translate("UnwrapGUI", "&Close", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileOpenMask.setText(QtGui.QApplication.translate("UnwrapGUI", "Open &mask", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuPhaseUnwrapping.setText(QtGui.QApplication.translate("UnwrapGUI", "Phase &unwrapping", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuPhaseDemodulation.setText(QtGui.QApplication.translate("UnwrapGUI", "Phase &demodulation", None, QtGui.QApplication.UnicodeUTF8))

