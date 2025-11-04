from PySide6.QtWidgets import QFrame, QWidget, QLabel, QVBoxLayout


class IOMonitorPanel(QWidget):
    def __init__(self):
        super().__init__()
        self.setObjectName("monitor-panel")
        self.setStyleSheet("border: none;")
        io_layout = QVBoxLayout(self)
        io_layout.addWidget(QLabel("I/O Monitor (placeholder)"))
