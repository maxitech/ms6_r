from PySide6.QtWidgets import QWidget, QVBoxLayout, QLabel


class SettingsPage(QWidget):
    def __init__(self):
        super().__init__()
        s_layout = QVBoxLayout(self)
        s_layout.addWidget(QLabel("General Settings Page (full-window)"))
