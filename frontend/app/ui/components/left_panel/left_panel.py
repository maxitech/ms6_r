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
            """
        self.setStyleSheet(style)

    def _create_layout(self):
        self.setObjectName("left-panel")
        self.setFixedWidth(345)
        left_layout = QVBoxLayout(self)

        con_panel = QFrame()
        con_panel.setObjectName("con-panel")
        con_panel.setFixedHeight(235)
        layout_con_panel = QVBoxLayout(con_panel)

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
        label_con_status = QLabel("Disconnected")
        label_con_status.setObjectName("label-con-status")
        layout_con_div.addWidget(label_con)
        layout_con_div.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        )
        layout_con_div.addWidget(label_con_status)

        layout_con_panel.addWidget(con_div)

        # Connect Btn
        con_btn = QPushButton("Connect")
        con_btn.setObjectName("btn-con")
        layout_con_panel.addWidget(con_btn)

        # Emergency Btn
        emergency_stop_btn = QPushButton("EMERGENCY STOP")
        emergency_stop_btn.setObjectName("btn-emergency-stop")
        layout_con_panel.addWidget(emergency_stop_btn)

        com_port_label = QLabel("COM Port:")
        com_port_label.setObjectName("label-com-port")
        com_port_label.setIndent(0)
        layout_con_panel.addWidget(com_port_label)

        combo_box = QComboBox()
        combo_box.setObjectName("combo-box-comport")
        combo_box.addItem("COM1")
        combo_box.addItem("COM2")
        combo_box.addItem("COM3")
        layout_con_panel.addWidget(combo_box)

        # layout_con_panel.addSpacerItem(
        #     QSpacerItem(0, 0, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        # )

        # layout_con_panel.addSpacerItem(
        #     QSpacerItem(0, 0, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        # )
        ctrl_panel = QFrame()

        left_layout.addWidget(con_panel)
        left_layout.addWidget(ctrl_panel)
