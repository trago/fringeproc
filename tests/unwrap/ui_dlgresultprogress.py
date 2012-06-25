# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'dlgresultprogress.ui'
#
# Created: Sun Jun 24 22:21:44 2012
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_DlgProgressResult(object):
    def setupUi(self, DlgProgressResult):
        DlgProgressResult.setObjectName(_fromUtf8("DlgProgressResult"))
        DlgProgressResult.resize(947, 812)
        self.verticalLayout = QtGui.QVBoxLayout(DlgProgressResult)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.graphicsView = QtGui.QGraphicsView(DlgProgressResult)
        self.graphicsView.setObjectName(_fromUtf8("graphicsView"))
        self.verticalLayout.addWidget(self.graphicsView)
        self.btnAcceptCancel = QtGui.QDialogButtonBox(DlgProgressResult)
        self.btnAcceptCancel.setOrientation(QtCore.Qt.Horizontal)
        self.btnAcceptCancel.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.btnAcceptCancel.setObjectName(_fromUtf8("btnAcceptCancel"))
        self.verticalLayout.addWidget(self.btnAcceptCancel)

        self.retranslateUi(DlgProgressResult)
        QtCore.QObject.connect(self.btnAcceptCancel, QtCore.SIGNAL(_fromUtf8("accepted()")), DlgProgressResult.accept)
        QtCore.QObject.connect(self.btnAcceptCancel, QtCore.SIGNAL(_fromUtf8("rejected()")), DlgProgressResult.reject)
        QtCore.QMetaObject.connectSlotsByName(DlgProgressResult)

    def retranslateUi(self, DlgProgressResult):
        DlgProgressResult.setWindowTitle(QtGui.QApplication.translate("DlgProgressResult", "Dialog", None, QtGui.QApplication.UnicodeUTF8))

