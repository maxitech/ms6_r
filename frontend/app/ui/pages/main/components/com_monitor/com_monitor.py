from PySide6.QtWidgets import (
    QFrame,
    QWidget,
    QLabel,
    QVBoxLayout,
    QWidget,
    QVBoxLayout,
    QScrollArea,
    QFrame,
    QLabel,
    QPushButton,
    QSpacerItem,
    QSizePolicy,
    QHBoxLayout,
    QComboBox,
    QCheckBox,
    QGroupBox,
)
from PySide6.QtCore import Qt, QTime


class ComMonitorPanel(QWidget):
    def __init__(self):
        super().__init__()
        self._setup_style()
        self._create_layout()

        self._raw_data_labels: list[QLabel] = []
        self._parsed_labels: list[QLabel] = []

    def _setup_style(self):
        bg_color = "#e5e7eb"
        col_text = "#000000"
        col_text_disabled = "#6b7280"
        col_light_gray = "#d1d5dc"
        col_mid_gray = "#99a1af"
        col_hover_gray = "#bdc2cc"

        style = f"""
        QFrame#top-container {{
            border: 2px solid {col_mid_gray};
        }}
        
        QFrame#top-container QLabel, QFrame#top-container QGroupBox QCheckBox {{
            color: {col_text};
        }}
        
        QFrame#top-container QLabel#h1 {{
            font-size: 16px;
            font-weight: 700;
            border: none;
        }}

        QFrame#top-container QGroupBox {{
            border: none;
        }}
        
        QFrame#top-container QGroupBox QCheckBox {{
            background-color: {bg_color};
            font-size: 12px;
            font-weight: 700;
        }}
        
        QFrame#top-container QFrame#top-inner-div {{
            border: none;
            border-top: 1px solid {col_mid_gray};
            padding: 0;
        }}
        
        QFrame#top-container QFrame#top-inner-div QLabel {{
            padding: 0;
            max-height: 20px;
            border: none;
            font-size: 12px;
            font-weight: 700;
        }}
        
        QFrame#top-container QFrame#top-inner-div QPushButton {{
            padding: 4px 16px;
            padding-top: 6px;
            font-size: 12px;
            font-weight: 700;
        }}
        
        QFrame#log-container {{
           border: none;
           border-left: 4px solid {col_mid_gray};
        }}
        
        QFrame#log-container[logType="info"] {{
            border-left: 4px solid {col_mid_gray};
        }}

        QFrame#log-container[logType="data"] {{
            border-left: 4px solid #193CB8;
        }}

        QFrame#log-container[logType="error"] {{
            border-left: 4px solid #9F0712;
        }}

        QFrame#log-container[logType="warning"] {{
            border-left: 4px solid #ff7b00;
        }}
    
        QLabel#dir-label {{
            font-size: 11px;
            padding: 2px 4px 0 4px;
            font-weight: 700;
            border: 2px solid {col_mid_gray};
            border-radius: 5px;          
        }}
        
        QLabel#dir-label[logType="sys"] {{
            background-color: #d1d5dc;
            color: {col_text};
        }}

        QLabel#dir-label[logType="tx"] {{
            background-color: #BEDBFF;
            color: #193CB8;
        }}

        QLabel#dir-label[logType="rx"] {{
            background-color: #B9F8CF;
            color: #016630;
        }}

        QLabel#dir-label[logType="error"] {{
            background-color: #FFC9C9;
            color: #9F0712;
        }}

        QLabel#type-label {{
            padding: 2px 4px 0 4px;
            font-size: 11px;
            font-weight: 700;
            border: 2px solid {col_mid_gray};
            border-radius: 3px;
        }}

        QLabel#type-label[logType="info"] {{
            background-color: {col_light_gray};
            color: {col_text};
        }}

        QLabel#type-label[logType="data"] {{
            background-color: #BEDBFF;
            color: #193CB8;
        }}

        QLabel#type-label[logType="error"] {{
            background-color: #FFC9C9;
            color: #9F0712;
        }}

        QLabel#type-label[logType="warning"] {{
            background-color: #fff8d6;
            color: #ff7b00;
        }}


        QFrame#log-container QLabel#msg-label {{
            border: none;
            font-size: 12px;
            font-weight: 700;
            color: {col_text};
        }}
        
        QFrame#log-container QLabel#raw-data-label {{
            color: #193CB8;
            font-size: 12px;
            padding: 2px 2px;
            padding-top: 4px;
            border: 1px solid {col_light_gray};
        }}
        
        QFrame#log-container QLabel#parsed-label {{
            color: #016630;
            font-weight: 700;
            font-size: 12px;
            padding: 2px 2px;
            padding-top: 4px;
            border: 1px solid {col_light_gray};
        }}
        """
        self.setStyleSheet(style)

    def _create_layout(self):
        # Top Container
        top_container = QFrame()
        top_container.setObjectName("top-container")
        top_container_layout = QVBoxLayout(top_container)
        top_container_layout.setContentsMargins(8, 8, 8, 0)
        h1 = QLabel("Communication Monitor")
        h1.setObjectName("h1")

        # Top inner div(functional elements)
        top_inner_div = QFrame()
        top_inner_div.setObjectName("top-inner-div")
        top_inner_div_layout = QHBoxLayout(top_inner_div)
        top_inner_div_layout.setContentsMargins(0, 0, 0, 0)

        dir_box = QWidget()
        dir_box_layout = QHBoxLayout(dir_box)
        dir_box_layout.setContentsMargins(0, 0, 0, 0)
        dir_label = QLabel("Direction:")
        dir_label.setIndent(0)
        dir_box_layout.addWidget(dir_label)
        dir_combo_box = QComboBox()
        dir_combo_box.addItem("All")
        dir_combo_box.addItem("TX")
        dir_combo_box.addItem("RX")
        dir_combo_box.addItem("SYS")
        dir_box_layout.addWidget(dir_combo_box)

        type_box = QWidget()
        type_box_layout = QHBoxLayout(type_box)
        type_label = QLabel("Type:")
        type_label.setIndent(0)
        type_box_layout.addWidget(type_label)
        type_combo_box = QComboBox()
        type_combo_box.addItem("All")
        type_combo_box.addItem("Data")
        type_combo_box.addItem("Error")
        type_combo_box.addItem("Info")
        type_combo_box.addItem("Warning")
        type_box_layout.addWidget(type_combo_box)

        check_box_group = QGroupBox()
        check_box_group_layout = QHBoxLayout(check_box_group)
        check_box_group_layout.setContentsMargins(0, 0, 0, 0)
        self._raw_data_check_box = QCheckBox("Raw Data")
        self._raw_data_check_box.setChecked(True)
        self._raw_data_check_box.toggled.connect(self._on_raw_data_toggled)
        parsed_check_box = QCheckBox("Parsed")
        parsed_check_box.setChecked(True)
        self._auto_scroll_check_box = QCheckBox("Auto Scroll")
        self._auto_scroll_check_box.setChecked(True)
        self._auto_scroll_check_box.toggled.connect(self._on_auto_scroll_toggled)
        check_box_group_layout.addWidget(self._raw_data_check_box)
        check_box_group_layout.addWidget(parsed_check_box)
        check_box_group_layout.addWidget(self._auto_scroll_check_box)

        clear_btn = QPushButton("Clear")

        # Add elements to inner div
        top_inner_div_layout.addWidget(dir_box)
        top_inner_div_layout.addWidget(type_box)
        top_inner_div_layout.addWidget(check_box_group)
        top_inner_div_layout.addStretch()
        top_inner_div_layout.addWidget(clear_btn)

        # Add elements to top container
        top_container_layout.addWidget(h1)
        top_container_layout.addWidget(top_inner_div)
        # top_container_layout.addStretch()

        # Scroll Area
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)

        bar = self.scroll_area.verticalScrollBar()
        bar.rangeChanged.connect(self.ResizeScroll)

        # Content Widget inside Scroll Area
        content_widget = QWidget()
        self.content_layout = QVBoxLayout(content_widget)
        self.content_layout.addStretch()

        self.scroll_area.setWidget(content_widget)

        # Main Layout
        log_layout = QVBoxLayout(self)
        log_layout.addWidget(top_container)
        log_layout.addWidget(self.scroll_area)

    def add_log_entry(self, text):
        frame = QFrame()
        frame.setObjectName("log-container")
        frame_layout = QVBoxLayout(frame)

        info_div = QFrame()
        info_div.setStyleSheet("padding-top: 2px; border: none;")
        info_div_layout = QHBoxLayout(info_div)
        info_div_layout.setContentsMargins(0, 0, 0, 0)
        curr_t = QTime.currentTime().toString("hh:mm:ss.zzz AP")
        time_label = QLabel(f"{curr_t}")
        time_label.setIndent(0)
        time_label.setStyleSheet("color: #343a40; font-size: 10px; font-weight: 700;")
        info_div_layout.addWidget(time_label)

        dir_label = QLabel()
        dir_label.setObjectName("dir-label")
        dir_label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        valid_dirs = {"sys", "tx", "rx"}
        d = dir.lower().strip()
        if d in valid_dirs:
            dir_label.setProperty("logType", d)
            dir_label.setText(f"{dir.upper()}")
        else:
            dir_label.setProperty("logType", "error")
            dir_label.setText("ERROR: Unknown dir label!")

        dir_label.setStyleSheet(self.styleSheet())

        # if d == "sys":
        #     pass
        # elif d == "tx":
        #     pass
        # elif d == "rx":
        #     pass
        # else:
        #     pass

        type_label = QLabel()
        type_label.setObjectName("type-label")
        type_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        valid_types = {"data", "error", "info", "warning"}
        t = type.lower().strip()

        if t in valid_types:
            type_label.setText(f"{type.upper()}")
            type_label.setProperty("logType", t)
            # frame.setProperty("logType", t)
        else:
            type_label.setProperty("logType", "error")
            # frame.setProperty("logType", "error")
            type_label.setText("ERROR: Unknown type label!")
        type_label.setStyleSheet(self.styleSheet())

        # if t == "info":
        #     pass
        # elif t == "data":
        #     pass
        # elif t == "error":
        #     pass
        # elif t == "warning":
        #     pass
        # else:
        #     pass

        info_div_layout.addWidget(dir_label)
        info_div_layout.addWidget(type_label)
        info_div_layout.addStretch()

        msg_label = QLabel(f"{msg}")
        msg_label.setObjectName("msg-label")

        # Toggle
        self._raw_data_label = QLabel()
        self._raw_data_label.setIndent(0)
        self._raw_data_label.setObjectName("raw-data-label")
        self._raw_data_labels.append(self._raw_data_label)
        if not isinstance(received_bytes, (bytes, bytearray)):
            received_bytes = b""
        try:
            raw_data_str = "Raw: " + " ".join(f"{b:02X}" for b in received_bytes)
        except Exception as e:
            raw_data_str = f"Raw: <error: {e}>"
        self._raw_data_label.setText(raw_data_str)

        self._raw_data_label.setVisible(True)

        frame_layout.addWidget(info_div)
        frame_layout.addWidget(msg_label)
        frame_layout.addWidget(self._raw_data_label)
        self.content_layout.insertWidget(self.content_layout.count() - 1, frame)

    def ResizeScroll(self, _, max):
        if self._auto_scroll_check_box.isChecked():
            self.scroll_area.verticalScrollBar().setValue(max)

    def _on_raw_data_toggled(self, checked: bool):
        for lbl in self._raw_data_labels:
            lbl.setVisible(checked)

    def _on_auto_scroll_toggled(self, checked: bool):
        if checked:
            bar = self.scroll_area.verticalScrollBar()
            bar.setValue(bar.maximum())
