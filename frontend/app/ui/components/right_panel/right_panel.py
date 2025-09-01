from PySide6.QtWidgets import (
    QFrame,
    QLabel,
    QVBoxLayout,
    QHBoxLayout,
    QPushButton,
    QButtonGroup,
    QSpacerItem,
    QSizePolicy,
)


class RightPanel(QFrame):
    def __init__(self):
        super().__init__()
        self._setup_style()
        self._create_panel()

    def _setup_style(self):
        bg_color = "#e5e7eb"
        col_text = "#000000"
        col_text_disabled = "#6b7280"
        col_light_gray = "#d1d5dc"
        col_mid_gray = "#99a1af"
        col_hover_gray = "#bdc2cc"

        style = f"""
            QFrame#program-ctrl QLabel {{
                color: {col_text};
                font-weight: 500;
                border: none;
            }}
            
            QLabel#label-prog-ctrl {{
                font-size: 16px;
            }}
            
            QLabel#label-rbt-op {{
                font-size: 13px;
            }}
            
            QFrame#program-ctrl QFrame#btn-container {{
                padding: 0;
                margin: 0;
                border: none;
            }}
            """
        self.setStyleSheet(style)

    def _create_panel(self):
        self.setObjectName("right-panel")
        self.setFixedWidth(375)
        right_layout = QVBoxLayout(self)

        program_ctrl = QFrame()
        program_ctrl.setObjectName("program-ctrl")
        layout_prog_ctrl = QVBoxLayout(program_ctrl)

        # Label 1
        label_prog_ctrl = QLabel("Program Control")
        label_prog_ctrl.setObjectName("label-prog-ctrl")
        layout_prog_ctrl.addWidget(label_prog_ctrl)

        # Label 2
        label_rbt_op = QLabel("Robot Operations")
        label_rbt_op.setObjectName("label-rbt-op")
        layout_prog_ctrl.addWidget(label_rbt_op)

        # Container for btn's
        op_btn_div = QFrame()
        op_btn_div.setObjectName("btn-container")
        op_btn_div.setFrameShape(QFrame.Shape.NoFrame)
        layout_op_btn_div = QVBoxLayout(op_btn_div)
        layout_op_btn_div.setContentsMargins(0, 0, 0, 0)
        layout_op_btn_div.setSpacing(5)

        # Row 1
        op_btn_div_row1 = QHBoxLayout()
        run_btn = QPushButton("Run")
        run_btn.setObjectName("btn-run")
        run_btn.setCheckable(True)
        run_btn.setChecked(True)
        run_btn.setEnabled(False)
        op_btn_div_row1.addWidget(run_btn)

        stop_btn = QPushButton("Stop")
        stop_btn.setObjectName("btn-stop")
        stop_btn.setCheckable(True)
        stop_btn.setEnabled(False)
        op_btn_div_row1.addWidget(stop_btn)

        group = QButtonGroup(op_btn_div)
        group.setExclusive(True)
        group.addButton(run_btn)
        group.addButton(stop_btn)

        step_btn = QPushButton("Step")
        step_btn.setObjectName("btn-step")
        step_btn.setEnabled(False)
        op_btn_div_row1.addWidget(step_btn)

        # Row 2
        op_btn_div_row2 = QHBoxLayout()
        load_btn = QPushButton("Load Program to Robot")
        load_btn.setObjectName("btn-load-to-rbt")
        load_btn.setEnabled(False)
        op_btn_div_row2.addWidget(load_btn)

        # Add rows to frame layout
        layout_op_btn_div.addLayout(op_btn_div_row1)
        layout_op_btn_div.addLayout(op_btn_div_row2)

        # Add frame to main layout
        layout_prog_ctrl.addWidget(op_btn_div)

        layout_prog_ctrl.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        )
        right_layout.addWidget(program_ctrl)
