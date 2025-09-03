from PySide6.QtWidgets import QWidget, QVBoxLayout, QLabel


class AboutPage(QWidget):
    def __init__(self):
        super().__init__()
        a_layout = QVBoxLayout(self)
        a_layout.addWidget(QLabel("About Page (full-window)"))
