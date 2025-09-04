from PySide6.QtWidgets import (
    QFrame,
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
    QRadioButton,
    QButtonGroup,
    QSpacerItem,
    QSizePolicy,
    QPushButton,
    QComboBox,
    QSlider,
    QGroupBox,
    QStackedWidget,
    QGridLayout,
)


from PySide6 import QtCore


class CtrlPanel(QFrame):
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
            QFrame#ctrl-panel {{
                border: 2px solid {col_mid_gray}
            }}
        
            QLabel#h1 {{
                font-size: 16px;
                font-weight: 500;
                margin-bottom: 8px;
            }}
        
            QGroupBox {{
                background-color: transparent;
                color: {col_text};
                border: none;
                font-size: 14px;
                font-weight: 400;
                margin-top: 28px;
                margin-bottom: 8px;
            }}
            
            QRadioButton {{
                color: {col_text};
                font-size: 14px;
                font-weight: 400;
            }}
            
            QRadioButton::indicator {{
                width: 14px;
                height: 14px;
                border-radius: 7px;
                border: 2px solid {col_mid_gray};
                background-color: transparent;
            }}
            
            QRadioButton::indicator::unchecked{{
                border: 1px solid; 
                border-color: {col_mid_gray};
                border-radius: 5px;
                background-color: white; 
                width: 11px; 
                height: 11px; 
            }}

            QRadioButton::indicator::checked{{
                border: 3px solid; 
                border-color: white;
                border-radius: 6px;
                background-color: #0074bc; 
                width: 7px; 
                height: 7px; 
            }}
        
            QFrame#slider-div {{
                border: none;
            }}

            QFrame#slider-div QLabel {{
                font-size: 14px;
                margin-bottom: 4px;
                margin-top: 8px;
                font-weight: 400;
            }}

            QFrame#slider-div QFrame#label-slider {{
                margin-right: 12px;
            }}

            QSlider::groove:horizontal {{
                border: 1px solid #bbb;
                background: white;
                height: 10px;
                border-radius: 4px;
            }}

            QSlider::handle:horizontal {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #eee, stop:1 #ccc);
                border: 1px solid #777;
                width: 13px;
                margin-top: -2px;
                margin-bottom: -2px;
                border-radius: 4px;
            }}

            QSlider::handle:horizontal:hover {{
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #fff, stop:1 #ddd);
                border: 1px solid #444;
                border-radius: 4px;
            }}

            QSlider::add-page:horizontal {{
                background: #fff;
                border: 1px solid #777;
                height: 10px;
                border-radius: 4px;
            }}

            QSlider::sub-page:horizontal {{   
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                    stop: 0 #66b8e3, stop: 1 #0074bc);
                background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,
                    stop: 0 #66b8e3, stop: 1 #0074bc);
                border: 1px solid #777;
                height: 10px;
                border-radius: 4px;
            }}

            QSlider::sub-page:horizontal:disabled {{
                background: #bbb;
                border-color: #999;
            }}

            QSlider::add-page:horizontal:disabled {{
                background: #eee;
                border-color: #999;
            }}

            QSlider::handle:horizontal:disabled {{
                background: #eee;
                border: 1px solid #aaa;
                border-radius: 4px;
            }}

            QSlider#jog-speed-slider {{
                margin-bottom: 18px;
            }}

            QStackedWidget , QStackedWidget QFrame{{
                border: none;
            }}
            
            QStackedWidget QFrame QPushButton {{
                min-height: 0;
                font-size: 14px;
                font-weight: 700;
            }}
            
            QStackedWidget QFrame QPushButton#btn-home-pos {{
                font-weight: 400;
                font-size: 14px;
            }}
            """
        self.setStyleSheet(style)

    def _create_layout(self):
        self.setObjectName("ctrl-panel")
        layout_ctrl_panel = QVBoxLayout(self)

        h1 = QLabel("Manual Control")
        h1.setObjectName("h1")
        h1.setIndent(0)
        layout_ctrl_panel.addWidget(h1)

        group_box = QGroupBox("Jog Mode:")
        layout_group = QHBoxLayout(group_box)
        layout_group.setContentsMargins(0, 0, 0, 0)
        radio1 = QRadioButton("Joint")
        radio1.setChecked(True)
        radio2 = QRadioButton("Cartesian")
        layout_group.addWidget(radio1)
        layout_group.addWidget(radio2)
        layout_group.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        )
        layout_ctrl_panel.addWidget(group_box)

        slider_div = QFrame()
        slider_div.setObjectName("slider-div")
        layout_slider_div = QHBoxLayout(slider_div)
        layout_slider_div.setContentsMargins(0, 0, 0, 0)
        layout_slider_div.setSpacing(0)

        label_slider = QLabel("Jog Speed:")
        label_slider.setObjectName("label-slider")
        layout_slider_div.addWidget(label_slider)

        label_speed = QLabel("50%")
        layout_slider_div.addWidget(label_speed)
        layout_slider_div.addSpacerItem(
            QSpacerItem(0, 0, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        )
        layout_ctrl_panel.addWidget(slider_div)

        slider = QSlider(QtCore.Qt.Orientation.Horizontal)
        slider.setObjectName("jog-speed-slider")
        slider.setMinimum(0)
        slider.setMaximum(100)
        slider.setValue(50)
        slider.setSingleStep(10)
        slider.setTickInterval(10)
        layout_ctrl_panel.addWidget(slider)

        stacked = QStackedWidget()
        set1 = QFrame()
        layout_set1 = QGridLayout(set1)
        layout_set1.setContentsMargins(0, 0, 0, 0)
        layout_set1.setSpacing(8)
        for row in range(6):
            btn = QPushButton(f"J{row+1}+")
            btn.setObjectName(f"btn-jog-j{row+1}-pos")
            btn.setSizePolicy(
                QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding
            )
            layout_set1.addWidget(btn, row, 0)

        for row in range(6):
            btn = QPushButton(f"J{row+1}-")
            btn.setObjectName(f"btn-jog-j{row+1}-neg")
            btn.setSizePolicy(
                QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding
            )
            layout_set1.addWidget(btn, row, 1)
        stacked.addWidget(set1)

        set2 = QFrame()
        layout_set2 = QGridLayout(set2)
        layout_set2.setContentsMargins(0, 0, 0, 0)
        layout_set2.setSpacing(8)
        positive_btn_list = ["X+", "Y+", "Z+", "Rx+", "Ry+", "Rz+"]
        negative_btn_list = ["X-", "Y-", "Z-", "Rx-", "Ry-", "Rz-"]
        for row, btn in enumerate(positive_btn_list):
            btn = QPushButton(f"{btn}")
            btn.setObjectName(f"btn-jog-cart-{row+1}-pos")
            btn.setSizePolicy(
                QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding
            )
            layout_set2.addWidget(btn, row, 0)

        for row, btn in enumerate(negative_btn_list):
            btn = QPushButton(f"{btn}")
            btn.setObjectName(f"btn-jog-cart-{row+1}-neg")
            btn.setSizePolicy(
                QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding
            )
            layout_set2.addWidget(btn, row, 1)
        stacked.addWidget(set2)

        stacked.setCurrentIndex(0)
        radio1.toggled.connect(lambda: stacked.setCurrentIndex(0))
        radio2.toggled.connect(lambda: stacked.setCurrentIndex(1))
        layout_ctrl_panel.addWidget(stacked)

        btn_home_position = QPushButton("Home Position")
        btn_home_position.setObjectName("btn-home-pos")
        layout_ctrl_panel.addWidget(btn_home_position)

        # btn_save_position = QPushButton("Save Position")
        # btn_save_position.setObjectName("btn-save-pos")
        # layout_ctrl_panel.addWidget(btn_save_position)
