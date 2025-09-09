from PySide6.QtWidgets import (
    QFrame,
    QHBoxLayout,
    QSpacerItem,
    QSizePolicy,
    QLabel,
    QApplication,
)
from PySide6.QtCore import QTimer, QTime


def make_panel(name):
    panel = QFrame()
    panel.setFrameShape(QFrame.Shape.StyledPanel)
    panel.setObjectName(name)
    return panel


class BottomBar(QFrame):
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
            QFrame#btm-bar QLabel {{
                color: {col_text};
                font-size: 12px;
                border: none;
                padding: 0 12px;
            }}
            
            QFrame#btm-bar QLabel#label-con-status, QFrame#btm-bar QLabel#label-port, QFrame#btm-bar QLabel#label-mode {{
                border-right: 1px solid {col_mid_gray};
            }}

            QFrame#btm-bar QLabel#label-program {{
                border-left: 1px solid {col_mid_gray};
            }}

            QFrame#btm-bar QLabel#label-app-info {{
                border: none;
            }}

            QFrame#btm-bar QLabel#label-time {{
                border-left: 1px solid {col_mid_gray};
            }}
            """
        self.setStyleSheet(style)

    def _create_layout(self):
        self.setFixedHeight(25)
        self.setObjectName("btm-bar")
        layout_bar = QHBoxLayout(self)
        layout_bar.setSpacing(0)
        layout_bar.setContentsMargins(0, 5, 0, 5)

        self._label_status = QLabel()
        self._label_status.setObjectName("label-con-status")
        self._label_status.setText("Status: Disconnected")
        self._label_status.setContentsMargins(0, 0, 0, 0)
        layout_bar.addWidget(self._label_status)

        self._label_port = QLabel()
        self._label_port.setObjectName("label-port")
        self._label_port.setText("Port: None")
        self._label_port.setContentsMargins(0, 0, 0, 0)
        layout_bar.addWidget(self._label_port)

        self._label_mode = QLabel()
        self._label_mode.setObjectName("label-mode")
        self._label_mode.setText("Mode: JOINT")
        self._label_mode.setContentsMargins(0, 0, 0, 0)
        layout_bar.addWidget(self._label_mode)

        self._label_speed = QLabel()
        self._label_speed.setObjectName("label-speed")
        self._label_speed.setText("Speed: 50%")
        self._label_speed.setContentsMargins(0, 0, 0, 0)
        layout_bar.addWidget(self._label_speed)

        self._label_program = QLabel()
        self._label_program.setObjectName("label-program")
        self._label_program.setText("Selected Program: None")
        self._label_program.setContentsMargins(0, 0, 0, 0)
        layout_bar.addWidget(self._label_program)

        layout_bar.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        )

        app_instance = QApplication.instance()
        if app_instance is not None:
            version = app_instance.applicationVersion()
            app_name = app_instance.applicationName()
        else:
            version = "unknown"
            app_name = "MS6_R Software Suite"
        label_app_info = QLabel(f"{app_name} v{version}")
        label_app_info.setObjectName("label-app-info")
        label_app_info.setContentsMargins(0, 0, 0, 0)
        layout_bar.addWidget(label_app_info)

        self._label_time = QLabel()
        self._label_time.setObjectName("label-time")
        self._label_time.setContentsMargins(0, 0, 0, 0)
        timer = QTimer(self)
        timer.timeout.connect(self._update_time)
        timer.start(1000)
        self._update_time()
        layout_bar.addWidget(self._label_time)

    def _update_time(self):
        curr_t = QTime.currentTime().toString("hh:mm:ss AP")
        self._label_time.setText(curr_t)

    @property
    def con_status(self) -> QLabel:
        return self._label_status

    @con_status.setter
    def con_status(self, status: str):
        self._label_status.setText(f"Status: {status}")

    @property
    def com_port(self) -> QLabel:
        return self._label_port

    @com_port.setter
    def com_port(self, port: str):
        self._label_port.setText(f"Port: {port}")

    @property
    def mode(self) -> QLabel:
        return self._label_mode

    @mode.setter
    def mode(self, mode: str):
        self._label_mode.setText(f"Mode: {mode}")

    @property
    def speed(self) -> QLabel:
        return self._label_speed

    @speed.setter
    def speed(self, speed: int):
        self._label_speed.setText(f"Speed: {str(speed)}%")

    @property
    def program(self) -> QLabel:
        return self._label_program

    @program.setter
    def program(self, prog: str):
        self._label_program.setText(f"Selected Program: {prog}")
