from PyQt4.QtGui import QFileDialog, QVBoxLayout
from PyQt4.QtCore import Qt
from actiondialog import ActionDialog

class FileDialog(ActionDialog):
    def __init__(self, parent=None, caption = ""):
        super(FileDialog, self).__init__(parent)
        
        self._filedlg = QFileDialog(None, Qt.Widget)
        self._filedlg.setFileMode(QFileDialog.ExistingFile)
        layout = QVBoxLayout()
        layout.setSpacing(0)
        layout.setContentsMargins(0,0,0,0)
        layout.addWidget(self._filedlg)
        
        self.setLayout(layout)
        self.setWindowTitle(caption)
        
        self._filedlg.finished.connect(self.fileDlgDone)

    def fileDlgDone(self, result):
        self.done(result)