from PySide6.QtWidgets import (
    QFrame,
    QVBoxLayout,
)


from .con_panel import ConPanel


class LeftPanel(QFrame):
    def __init__(self):
        super().__init__()
        self._setup_style()
        self._create_layout()

    def _setup_style(self):
        bg_color = "#e5e7eb"
        col_text = "#000000"
        col_text_disabled = "#6b7280"
        col_light_gray = "#d1d5dc"
        col_mid_gray = "#99a1af"
        col_hover_gray = "#bdc2cc"

        style = f"""
            QFrame#left-panel QLabel {{
                color: {col_text};
                font-weight: 500;
                border: none;
            }}
            """
        self.setStyleSheet(style)

    def _create_layout(self):
        self.setObjectName("left-panel")
        self.setFixedWidth(345)
        left_layout = QVBoxLayout(self)

        left_layout.addWidget(ConPanel())

        ctrl_panel = QFrame()

        left_layout.addWidget(ctrl_panel)
