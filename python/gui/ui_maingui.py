# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'maingui.ui'
#
# Created: Mon Jan 30 22:49:08 2012
#      by: PyQt4 UI code generator 4.7.2
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.wDesktop = QtGui.QWidget(self.centralwidget)
        self.wDesktop.setObjectName("wDesktop")
        self.horizontalLayout = QtGui.QHBoxLayout(self.wDesktop)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.desktopLayout = QtGui.QHBoxLayout()
        self.desktopLayout.setObjectName("desktopLayout")
        self.horizontalLayout.addLayout(self.desktopLayout)
        self.verticalLayout.addWidget(self.wDesktop)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menuMain = QtGui.QMenuBar(MainWindow)
        self.menuMain.setGeometry(QtCore.QRect(0, 0, 800, 25))
        self.menuMain.setObjectName("menuMain")
        self.menuFile = QtGui.QMenu(self.menuMain)
        self.menuFile.setObjectName("menuFile")
        MainWindow.setMenuBar(self.menuMain)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionFileOpen = QtGui.QAction(MainWindow)
        self.actionFileOpen.setObjectName("actionFileOpen")
        self.menuFile.addAction(self.actionFileOpen)
        self.menuMain.addAction(self.menuFile.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "&File", None, QtGui.QApplication.UnicodeUTF8))
        self.actionFileOpen.setText(QtGui.QApplication.translate("MainWindow", "&Open", None, QtGui.QApplication.UnicodeUTF8))

