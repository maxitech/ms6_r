from PySide6.QtWidgets import QFrame, QWidget, QLabel, QVBoxLayout


class EditorPanel(QWidget):
    def __init__(self):
        super().__init__()
        self.setObjectName("editor-panel")
        self.setStyleSheet("border: none;")
        editor_layout = QVBoxLayout(self)
        editor_layout.addWidget(QLabel("Editor (placeholder)"))
