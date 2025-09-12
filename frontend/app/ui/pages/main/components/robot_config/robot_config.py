from PySide6.QtWidgets import (
    QFrame,
    QWidget,
    QLabel,
    QVBoxLayout,
    QPushButton,
    QHBoxLayout,
    QTabWidget,
    QGridLayout,
    QLabel,
    QFrame,
    QLineEdit,
)

from PySide6.QtCore import Qt


class RobotConfigPanel(QWidget):
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
            QFrame#top-frame {{
                border: none;
                border-bottom: 2px solid {col_mid_gray};
                padding-bottom: 4px;
                margin-bottom: 12px;
            }}
            
            QLabel#h1 {{
                font-size: 16px;
                font-weight: 700;
                color: {col_text};
                border: none;
            }}
            
            QPushButton#save-btn {{
                padding: 4px 6px 4px 6px;
            }}
            
            QTabBar::tab {{
                background: #F3F4F6;
                color: black;
                border: 1px solid {col_mid_gray};
                padding: 6px;
                border-radius: 2px;
            }}

            QTabBar::tab:selected {{
                background: white;
                color: black;
                border-bottom-left-radius: 0;
                border-bottom-right-radius: 0;
                border-bottom: none;
            }}
            
            QTabWidget::pane {{ 
                border: 1px solid {col_mid_gray};
            }}
            
            QLabel#tab-h1 {{
                color: {col_text};
                font-size: 16px;
                font-weight: 700;
                border: none;
                margin: 12px 0 12px 0;
            }}
            
            QFrame#table QLabel, QFrame#table QLineEdit {{
                color: {col_text};
                font-size: 14px;
                padding: 8px 0 8px 0;
            }}

            QLabel#corner {{
                border: 1px solid {col_mid_gray};
            }}
            
            QFrame#table QLabel#first-col {{
                padding: 0 6px 0 6px;
            }}
            
            QFrame#table QLineEdit {{ 
                border: 1px solid {col_mid_gray}; 
                background-color: #fff; 
            }}
            
            QToolTip {{
                border: 1px solid {col_mid_gray};
                padding: 0 2px 0 2px;
                border-radius: 3px;
                opacity: 200;
                color: {col_text};
                background-color: #F3F4F6;
            }}
        """
        self.setStyleSheet(style)

    def _create_layout(self):
        top_container = QFrame()
        top_container.setObjectName("top-frame")
        top_container_layout = QHBoxLayout(top_container)
        top_container_layout.setContentsMargins(0, 0, 0, 0)
        h1 = QLabel("Robot Configuration")
        h1.setObjectName("h1")

        self._save_btn = QPushButton("Save")
        self._save_btn.setObjectName("save-btn")

        top_container_layout.addWidget(h1)
        top_container_layout.addStretch()
        top_container_layout.addWidget(self._save_btn)

        tabs = QTabWidget()

        dh_tab = QWidget()
        tool_frame_tab = QWidget()
        home_pos_tab = QWidget()
        accel_tab = QWidget()

        dh_tab_layout = QVBoxLayout(dh_tab)
        dh_tab_h1 = QLabel("Denavit-Hartenberg Parameters")
        dh_tab_h1.setObjectName("tab-h1")
        dh_tab_layout.addWidget(dh_tab_h1)
        dh_tab_layout.addWidget(self._create_dh_table())
        dh_tab_layout.addStretch()

        tool_frame_tab_layout = QVBoxLayout(tool_frame_tab)
        tool_frame_tab_layout.addWidget(QLabel("TOOL FRAME TAB"))

        home_pos_tab_layout = QVBoxLayout(home_pos_tab)
        home_pos_tab_h1 = QLabel("Home Positions")
        home_pos_tab_h1.setObjectName("tab-h1")
        home_pos_inner_div = QFrame()
        home_pos_inner_div.setObjectName("hp-inner-div")
        hp_inner_div_layout = QHBoxLayout(home_pos_inner_div)
        hp_inner_div_layout.setContentsMargins(0, 0, 0, 0)
        hp_inner_div_layout.addWidget(self._create_home_pos_table())
        hp_inner_div_layout.addStretch()
        home_pos_tab_layout.addWidget(home_pos_tab_h1)
        home_pos_tab_layout.addWidget(home_pos_inner_div)
        home_pos_tab_layout.addStretch()

        tabs.addTab(dh_tab, "DH Parameters")
        tabs.addTab(tool_frame_tab, "Tool Frame")
        tabs.addTab(home_pos_tab, "Home Positions")
        tabs.addTab(accel_tab, "Max Accelerations")

        rbt_config_layout = QVBoxLayout(self)
        rbt_config_layout.addWidget(top_container)
        rbt_config_layout.addWidget(tabs)

    def _create_dh_table(self) -> QFrame:
        dh_table = QFrame()
        dh_table.setObjectName("dh-table")
        layout = QGridLayout(dh_table)
        self._dh_inputs: dict[tuple[int, str], QLineEdit] = {}  # Dict for read

        headers = [
            ("theta_offset", "θ (deg)"),
            ("alpha", "α (rad)"),
            ("d", "d (mm)"),
            ("a", "a (mm)"),
        ]
        tooltips = {
            "θ (deg)": "Theta offset (in degrees) default 0",
            "α (rad)": "Link twist (in radians)",
            "d (mm)": "Offset along previous z-axis",
            "a (mm)": "Length of common normal (link length)",
        }
        for col, (key, label_text) in enumerate(headers):
            label = QLabel(label_text)
            label.setAlignment(Qt.AlignmentFlag.AlignCenter)
            label.setToolTip(tooltips.get(label_text, ""))
            layout.addWidget(label, 0, col + 1)

        corner = QLabel("")  # Empty label for corner style
        corner.setObjectName("corner")
        layout.addWidget(corner, 0, 0)

        # Input fields for 6 Joints
        for row in range(6):
            joint_label = QLabel(f"Joint {row + 1}")
            joint_label.setObjectName("joint")
            layout.addWidget(joint_label, row + 1, 0)

            for col, (key, _) in enumerate(headers):
                field = QLineEdit()
                field.setAlignment(Qt.AlignmentFlag.AlignCenter)
                field.setText("0.0")
                layout.setSpacing(0)
                layout.setContentsMargins(0, 0, 0, 0)
                layout.addWidget(field, row + 1, col + 1)
                self._dh_inputs[(row, key)] = field

        return dh_table

    @property
    def dh_inputs(self) -> dict[tuple[int, str], QLineEdit]:
        return self._dh_inputs

    @property
    def save_btn(self) -> QPushButton:
        return self._save_btn
