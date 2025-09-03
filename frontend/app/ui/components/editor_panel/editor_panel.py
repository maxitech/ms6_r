from PySide6.QtWidgets import QFrame


class EditorPanel(QFrame):
    def __init__(self):
        super().__init__()
        self.setObjectName("editor-panel")
