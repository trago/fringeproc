# -*- coding: utf-8 -*-
# Copyright (c) 2012, Julio C. Estrada
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:

# + Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.

# + Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Form implementation generated from reading ui file 'mainwin.ui'
#
# Created: Mon Jun 25 22:40:27 2012
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_MainGUI(object):
    def setupUi(self, MainGUI):
        MainGUI.setObjectName(_fromUtf8("MainGUI"))
        MainGUI.resize(798, 600)
        self.centralwidget = QtGui.QWidget(MainGUI)
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
        MainGUI.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainGUI)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 798, 25))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self._mnuFile = QtGui.QMenu(self.menubar)
        self._mnuFile.setObjectName(_fromUtf8("_mnuFile"))
        self._mnuSystems = QtGui.QMenu(self.menubar)
        self._mnuSystems.setObjectName(_fromUtf8("_mnuSystems"))
        MainGUI.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainGUI)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainGUI.setStatusBar(self.statusbar)
        self._mnuFileOpen = QtGui.QAction(MainGUI)
        self._mnuFileOpen.setObjectName(_fromUtf8("_mnuFileOpen"))
        self._mnuFileSave = QtGui.QAction(MainGUI)
        self._mnuFileSave.setObjectName(_fromUtf8("_mnuFileSave"))
        self._mnuFileQuit = QtGui.QAction(MainGUI)
        self._mnuFileQuit.setObjectName(_fromUtf8("_mnuFileQuit"))
        self._mnuFileClose = QtGui.QAction(MainGUI)
        self._mnuFileClose.setObjectName(_fromUtf8("_mnuFileClose"))
        self._mnuFileOpenMask = QtGui.QAction(MainGUI)
        self._mnuFileOpenMask.setObjectName(_fromUtf8("_mnuFileOpenMask"))
        self._mnuPhaseUnwrapping = QtGui.QAction(MainGUI)
        self._mnuPhaseUnwrapping.setObjectName(_fromUtf8("_mnuPhaseUnwrapping"))
        self._mnuPhaseDemodulation = QtGui.QAction(MainGUI)
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

        self.retranslateUi(MainGUI)
        QtCore.QMetaObject.connectSlotsByName(MainGUI)

    def retranslateUi(self, MainGUI):
        MainGUI.setWindowTitle(QtGui.QApplication.translate("MainGUI", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFile.setTitle(QtGui.QApplication.translate("MainGUI", "&File", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuSystems.setTitle(QtGui.QApplication.translate("MainGUI", "&Systems", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileOpen.setText(QtGui.QApplication.translate("MainGUI", "&Open", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileSave.setText(QtGui.QApplication.translate("MainGUI", "&Save", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileQuit.setText(QtGui.QApplication.translate("MainGUI", "&Quit", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileClose.setText(QtGui.QApplication.translate("MainGUI", "&Close", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuFileOpenMask.setText(QtGui.QApplication.translate("MainGUI", "Open &mask", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuPhaseUnwrapping.setText(QtGui.QApplication.translate("MainGUI", "Phase &unwrapping", None, QtGui.QApplication.UnicodeUTF8))
        self._mnuPhaseDemodulation.setText(QtGui.QApplication.translate("MainGUI", "Phase &demodulation", None, QtGui.QApplication.UnicodeUTF8))

