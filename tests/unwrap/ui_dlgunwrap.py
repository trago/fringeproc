# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'dlgunwrap.ui'
#
# Created: Thu Jun 21 11:46:54 2012
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_DlgUnwrap(object):
    def setupUi(self, DlgUnwrap):
        DlgUnwrap.setObjectName(_fromUtf8("DlgUnwrap"))
        DlgUnwrap.resize(335, 250)
        self.verticalLayout = QtGui.QVBoxLayout(DlgUnwrap)
        self.verticalLayout.setSizeConstraint(QtGui.QLayout.SetFixedSize)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.label = QtGui.QLabel(DlgUnwrap)
        self.label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.lineTau = QtGui.QLineEdit(DlgUnwrap)
        self.lineTau.setObjectName(_fromUtf8("lineTau"))
        self.gridLayout.addWidget(self.lineTau, 0, 1, 1, 1)
        self.label_2 = QtGui.QLabel(DlgUnwrap)
        self.label_2.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)
        self.lineSmooth = QtGui.QLineEdit(DlgUnwrap)
        self.lineSmooth.setObjectName(_fromUtf8("lineSmooth"))
        self.gridLayout.addWidget(self.lineSmooth, 1, 1, 1, 1)
        self.label_3 = QtGui.QLabel(DlgUnwrap)
        self.label_3.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.gridLayout.addWidget(self.label_3, 2, 0, 1, 1)
        self.lineNeighbor = QtGui.QLineEdit(DlgUnwrap)
        self.lineNeighbor.setObjectName(_fromUtf8("lineNeighbor"))
        self.gridLayout.addWidget(self.lineNeighbor, 2, 1, 1, 1)
        self.verticalLayout.addLayout(self.gridLayout)
        self.groupBox = QtGui.QGroupBox(DlgUnwrap)
        self.groupBox.setFlat(False)
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.horizontalLayout = QtGui.QHBoxLayout(self.groupBox)
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.gridLayout_2 = QtGui.QGridLayout()
        self.gridLayout_2.setObjectName(_fromUtf8("gridLayout_2"))
        self.linePixelX = QtGui.QLineEdit(self.groupBox)
        self.linePixelX.setObjectName(_fromUtf8("linePixelX"))
        self.gridLayout_2.addWidget(self.linePixelX, 0, 1, 1, 1)
        self.label_5 = QtGui.QLabel(self.groupBox)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.gridLayout_2.addWidget(self.label_5, 2, 0, 1, 1)
        self.linePixelY = QtGui.QLineEdit(self.groupBox)
        self.linePixelY.setObjectName(_fromUtf8("linePixelY"))
        self.gridLayout_2.addWidget(self.linePixelY, 2, 1, 1, 1)
        self.label_4 = QtGui.QLabel(self.groupBox)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.gridLayout_2.addWidget(self.label_4, 0, 0, 1, 1)
        self.horizontalLayout.addLayout(self.gridLayout_2)
        self.pushFromImage = QtGui.QPushButton(self.groupBox)
        self.pushFromImage.setObjectName(_fromUtf8("pushFromImage"))
        self.horizontalLayout.addWidget(self.pushFromImage)
        self.verticalLayout.addWidget(self.groupBox)
        self.btnAceptCancel = QtGui.QDialogButtonBox(DlgUnwrap)
        self.btnAceptCancel.setOrientation(QtCore.Qt.Horizontal)
        self.btnAceptCancel.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.btnAceptCancel.setObjectName(_fromUtf8("btnAceptCancel"))
        self.verticalLayout.addWidget(self.btnAceptCancel)

        self.retranslateUi(DlgUnwrap)
        QtCore.QObject.connect(self.btnAceptCancel, QtCore.SIGNAL(_fromUtf8("accepted()")), DlgUnwrap.accept)
        QtCore.QObject.connect(self.btnAceptCancel, QtCore.SIGNAL(_fromUtf8("rejected()")), DlgUnwrap.reject)
        QtCore.QMetaObject.connectSlotsByName(DlgUnwrap)
        DlgUnwrap.setTabOrder(self.lineTau, self.lineSmooth)
        DlgUnwrap.setTabOrder(self.lineSmooth, self.lineNeighbor)
        DlgUnwrap.setTabOrder(self.lineNeighbor, self.linePixelX)
        DlgUnwrap.setTabOrder(self.linePixelX, self.linePixelY)
        DlgUnwrap.setTabOrder(self.linePixelY, self.pushFromImage)
        DlgUnwrap.setTabOrder(self.pushFromImage, self.btnAceptCancel)

    def retranslateUi(self, DlgUnwrap):
        DlgUnwrap.setWindowTitle(QtGui.QApplication.translate("DlgUnwrap", "Phase unwrapping parameters", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("DlgUnwrap", "tau:", None, QtGui.QApplication.UnicodeUTF8))
        self.lineTau.setText(QtGui.QApplication.translate("DlgUnwrap", "0.09", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("DlgUnwrap", "smooth:", None, QtGui.QApplication.UnicodeUTF8))
        self.lineSmooth.setText(QtGui.QApplication.translate("DlgUnwrap", "13", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("DlgUnwrap", "Neighbor size:", None, QtGui.QApplication.UnicodeUTF8))
        self.lineNeighbor.setText(QtGui.QApplication.translate("DlgUnwrap", "11", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("DlgUnwrap", "Starting pixel:", None, QtGui.QApplication.UnicodeUTF8))
        self.linePixelX.setText(QtGui.QApplication.translate("DlgUnwrap", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("DlgUnwrap", "Y:", None, QtGui.QApplication.UnicodeUTF8))
        self.linePixelY.setText(QtGui.QApplication.translate("DlgUnwrap", "0", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("DlgUnwrap", "X:", None, QtGui.QApplication.UnicodeUTF8))
        self.pushFromImage.setText(QtGui.QApplication.translate("DlgUnwrap", "&From image", None, QtGui.QApplication.UnicodeUTF8))

