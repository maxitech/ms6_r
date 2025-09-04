from PySide6.QtWidgets import (
    QFrame,
    QHBoxLayout,
    QVBoxLayout,
    QSpacerItem,
    QSizePolicy,
    QLabel,
    QPushButton,
)

from PySide6.QtCore import Qt


class CentralBottomPanel(QFrame):
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
            QFrame#central-btm-panel, QFrame#central-btm-panel QFrame {{
                background-color: {col_light_gray};
            }}
            
            QFrame#central-btm-panel QFrame, QFrame#central-btm-panel QLabel{{
                color: {col_text};
                border: none;
            }}
        
        
            QFrame#row-joint-pos QLabel {{
                color: {col_text};
            }}
            
            QFrame#central-btm-panel QLabel#h1 {{
                font-size: 16px;
                font-weight: 700;
            }}
            
            QFrame#central-btm-panel QPushButton#btn-teach-pos {{
                padding: 6px 12px;
            }}
            
            QFrame#central-btm-panel QLabel#label-joint-h2, QFrame#central-btm-panel QLabel#label-cart-h2 {{
                font-size: 11px;
                font-weight: 700;
                margin-bottom: 8px;
            }}
        
            """
        self.setStyleSheet(style)

    def _create_layout(self):
        self.setObjectName("central-btm-panel")
        self.setMaximumHeight(200)
        layout_ctl_btm = QVBoxLayout(self)
        layout_ctl_btm.setSpacing(0)

        top_row = QFrame()
        layout_top_row = QHBoxLayout(top_row)
        layout_top_row.setContentsMargins(0, 0, 0, 12)
        layout_top_row.setSpacing(0)

        h1 = QLabel("Current Position")
        h1.setObjectName("h1")
        layout_top_row.addWidget(h1)

        layout_top_row.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        )

        self._btn_teach_pos = QPushButton("Teach Position")
        self._btn_teach_pos.setObjectName("btn-teach-pos")
        self._btn_teach_pos.setEnabled(False)
        layout_top_row.addWidget(self._btn_teach_pos)
        layout_ctl_btm.addWidget(top_row)

        label_joint_h2 = QLabel("JOINT POSITIONS")
        label_joint_h2.setObjectName("label-joint-h2")
        layout_ctl_btm.addWidget(label_joint_h2)

        row_joint_pos = self._create_pos_row(mode=True)
        layout_ctl_btm.addWidget(row_joint_pos)

        layout_ctl_btm.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        )

        label_cart_h2 = QLabel("TOOL POSITION")
        label_cart_h2.setObjectName("label-cart-h2")
        layout_ctl_btm.addWidget(label_cart_h2)

        row_cart_pos = self._create_pos_row(mode=False)
        layout_ctl_btm.addWidget(row_cart_pos)

    def _create_pos_row(self, mode: bool) -> QFrame:
        row_joint_pos = QFrame()
        row_joint_pos.setObjectName("row-joint-pos")
        layout_row_joint_pos = QHBoxLayout(row_joint_pos)
        layout_row_joint_pos.setContentsMargins(0, 0, 0, 0)

        cart_list = ["X", "Y", "Z", "Rx", "Ry", "Rz"]

        for i in range(6):
            div_joint_pos = QFrame()
            div_joint_pos.setContentsMargins(0, 0, 0, 0)
            layout_div = QVBoxLayout(div_joint_pos)
            layout_div.setContentsMargins(0, 0, 0, 0)
            layout_div.setSpacing(0)

            div_h2 = QLabel()
            div_h2.setText(f"J{i+1}" if mode else f"{cart_list[i]}")
            div_h2.setStyleSheet(
                "font-size: 12px; font-weight: 700; margin-bottom: 4px;"
            )
            div_h2.setAlignment(Qt.AlignmentFlag.AlignCenter)
            layout_div.addWidget(div_h2)

            label_div_pos = QLabel()
            label_div_pos.setText("0.0°" if mode else f"0.0{' mm' if i < 3 else '°'}")
            label_div_pos.setAlignment(Qt.AlignmentFlag.AlignCenter)
            label_div_pos.setContentsMargins(0, 0, 0, 0)
            label_div_pos.setStyleSheet(
                "border: 1px solid #99a1af; background-color: #fff; padding-top: 2px; min-height: 18px;"
            )
            layout_div.addWidget(label_div_pos)

            layout_row_joint_pos.addWidget(div_joint_pos)

        return row_joint_pos

    @property
    def btn_teach_pos(self) -> QPushButton:
        return self._btn_teach_pos
