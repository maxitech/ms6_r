from PySide6.QtWidgets import QFrame


def make_panel(name):
    panel = QFrame()
    panel.setFrameShape(QFrame.Shape.StyledPanel)
    panel.setObjectName(name)
    return panel


class BottomBar(QFrame):
    def __init__(self):
        super().__init__()
        self.setFixedHeight(40)
        self.setObjectName("btm-bar")
