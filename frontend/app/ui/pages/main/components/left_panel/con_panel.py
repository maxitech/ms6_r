from PySide6.QtWidgets import (
    QFrame,
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
    QSpacerItem,
    QSizePolicy,
    QPushButton,
    QComboBox,
)


class ConPanel(QFrame):
    def __init__(self):
        super().__init__()
        self._setup_style()
        self._create_layout()

    def _setup_style(self):
        style = f"""
            QLabel#label-rbt-status {{
                font-size: 16px;
            }}
        
            QFrame#con-div {{
                border: none;
            }}
            
            QFrame#con-div QLabel {{
                font-size: 14px;
                font-weight: 400;
                margin-top: 4px;
            }}
            
            QPushButton#btn-con {{
                margin-top: 6px;
                margin-bottom: 2px;
            }}
            
            QPushButton#btn-emergency-stop {{
                margin-bottom: 0px;
            }}
            
            QLabel#label-com-port {{
                font-size: 14px;
                font-weight: 400;
                margin-top: 4px;
                padding: 0;
            }}
            
            QComboBox#combo-box-comport {{
                min-height: 30px;
                padding-left: 8px;
            }}
            """
        self.setStyleSheet(style)

    def _create_layout(self):
        self.setObjectName("con-panel")
        self.setFixedHeight(235)
        layout_con_panel = QVBoxLayout(self)

        label_rbt_status = QLabel("Robot Status")
        label_rbt_status.setObjectName("label-rbt-status")
        layout_con_panel.addWidget(label_rbt_status)
        layout_con_panel.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        )

        # Connection Status container
        con_div = QFrame()
        con_div.setObjectName("con-div")
        layout_con_div = QHBoxLayout(con_div)
        layout_con_div.setContentsMargins(0, 0, 0, 0)

        layout_con_panel.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        )

        # Connection Display
        label_con = QLabel("Connection:")
        self._label_con_status = QLabel("Disconnected")
        self._label_con_status.setObjectName("label-con-status")
        layout_con_div.addWidget(label_con)
        layout_con_div.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        )
        layout_con_div.addWidget(self._label_con_status)

        layout_con_panel.addWidget(con_div)

        # Connect Btn
        self._con_btn = QPushButton("Connect")
        self._con_btn.setObjectName("btn-con")
        layout_con_panel.addWidget(self._con_btn)

        # Emergency Btn
        self._emergency_stop_btn = QPushButton("EMERGENCY STOP")
        self._emergency_stop_btn.setObjectName("btn-emergency-stop")
        self._emergency_stop_btn.setEnabled(False)
        layout_con_panel.addWidget(self._emergency_stop_btn)

        com_port_label = QLabel("COM Port:")
        com_port_label.setObjectName("label-com-port")
        com_port_label.setIndent(0)
        layout_con_panel.addWidget(com_port_label)

        self._combo_box = QComboBox()
        self._combo_box.setObjectName("combo-box-comport")
        layout_con_panel.addWidget(self._combo_box)

    @property
    def emergency_stop_btn(self) -> QPushButton:
        return self._emergency_stop_btn

    @property
    def con_status_label(self) -> QLabel:
        return self._label_con_status

    @property
    def con_btn(self) -> QPushButton:
        return self._con_btn

    @property
    def combo_box(self) -> QComboBox:
        return self._combo_box
