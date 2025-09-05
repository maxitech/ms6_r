from PySide6.QtWidgets import QFrame, QWidget, QLabel, QVBoxLayout


class ComMonitorPanel(QWidget):
    def __init__(self):
        super().__init__()
        self.setObjectName("monitor-panel")
        self.setStyleSheet("border: none;")
        log_layout = QVBoxLayout(self)
        log_layout.addWidget(QLabel("Com Monitor (placeholder)"))
