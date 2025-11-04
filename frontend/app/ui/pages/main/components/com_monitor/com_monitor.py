from PySide6.QtWidgets import (
    QFrame,
    QWidget,
    QLabel,
    QVBoxLayout,
    QListWidget,
    QListWidgetItem,
    QPushButton,
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
            background-color: {bg_color};
        }}
        
        QFrame#top-container QFrame{{
            background-color: {bg_color};
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
        
        QListWidget#log-list {{
            background-color: #fff;
            border: 2px solid {col_mid_gray};
            padding: 4px;
        }}

        QListWidget#log-list::item {{
            background-color: #F9FAFB;
            border: none;
            border-left: 2px solid #99A1AF;
            padding: 0;
            margin-bottom: 4px;
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
        self._dir_combo_box = QComboBox()
        self._dir_combo_box.addItem("All")
        self._dir_combo_box.addItem("TX")
        self._dir_combo_box.addItem("RX")
        self._dir_combo_box.addItem("SYS")
        self._dir_combo_box.currentTextChanged.connect(self._on_filter_changed)
        dir_box_layout.addWidget(self._dir_combo_box)

        type_box = QWidget()
        type_box_layout = QHBoxLayout(type_box)
        type_label = QLabel("Type:")
        type_label.setIndent(0)
        type_box_layout.addWidget(type_label)
        self._type_combo_box = QComboBox()
        self._type_combo_box.addItem("All")
        self._type_combo_box.addItem("Data")
        self._type_combo_box.addItem("Error")
        self._type_combo_box.addItem("Info")
        self._type_combo_box.addItem("Warning")
        self._type_combo_box.currentTextChanged.connect(self._on_filter_changed)
        type_box_layout.addWidget(self._type_combo_box)

        check_box_group = QGroupBox()
        check_box_group_layout = QHBoxLayout(check_box_group)
        check_box_group_layout.setContentsMargins(0, 0, 0, 0)
        self._raw_data_check_box = QCheckBox("Raw Data")
        self._raw_data_check_box.setChecked(True)
        self._raw_data_check_box.toggled.connect(self._on_raw_data_toggled)
        self._parsed_check_box = QCheckBox("Parsed")
        self._parsed_check_box.setChecked(True)
        self._parsed_check_box.toggled.connect(self._on_parsed_toggled)
        self._auto_scroll_check_box = QCheckBox("Auto Scroll")
        self._auto_scroll_check_box.setChecked(True)
        self._auto_scroll_check_box.toggled.connect(self._on_auto_scroll_toggled)
        check_box_group_layout.addWidget(self._raw_data_check_box)
        check_box_group_layout.addWidget(self._parsed_check_box)
        check_box_group_layout.addWidget(self._auto_scroll_check_box)

        self._clear_btn = QPushButton("Clear")
        self._clear_btn.clicked.connect(self._on_clear_log_entries)

        # Add elements to inner div
        top_inner_div_layout.addWidget(dir_box)
        top_inner_div_layout.addWidget(type_box)
        top_inner_div_layout.addWidget(check_box_group)
        top_inner_div_layout.addStretch()
        top_inner_div_layout.addWidget(self._clear_btn)

        # Add elements to top container
        top_container_layout.addWidget(h1)
        top_container_layout.addWidget(top_inner_div)

        # List Widget
        self._log_list = QListWidget()
        self._log_list.setObjectName("log-list")
        self._log_list.setVerticalScrollMode(QListWidget.ScrollMode.ScrollPerPixel)

        # Main Layout
        log_layout = QVBoxLayout(self)
        log_layout.addWidget(top_container)
        log_layout.addWidget(self._log_list)

    def add_log_entry(
        self,
        dir: str,
        type: str,
        msg: str,
        raw_bytes: bytes | None = None,
        parsed: str = "",
    ):
        # Create the log entry widget
        log_widget = LogEntryWidget(dir, type, msg, raw_bytes, parsed)

        # Set initial visibility based on current checkbox states
        if log_widget.raw_data_label:
            log_widget.set_raw_data_visible(self._raw_data_check_box.isChecked())
        if log_widget.parsed_label:
            log_widget.set_parsed_visible(self._parsed_check_box.isChecked())

        # Create list item and add to list
        item = QListWidgetItem()
        item.setSizeHint(log_widget.sizeHint())

        self._log_list.addItem(item)
        self._log_list.setItemWidget(item, log_widget)

        # Check if item should be visible based on current filters
        self._filter_log_entries(
            self._dir_combo_box.currentText(), self._type_combo_box.currentText()
        )

        # Auto scroll if enabled
        if self._auto_scroll_check_box.isChecked():
            self._log_list.scrollToBottom()

    def _on_clear_log_entries(self):
        self._clear_btn.setEnabled(False)
        self._log_list.clear()
        self._clear_btn.setEnabled(True)

    def _on_filter_changed(self):
        selected_dir_filter = self._dir_combo_box.currentText()
        selected_type_filter = self._type_combo_box.currentText()
        self._filter_log_entries(selected_dir_filter, selected_type_filter)

    def _filter_log_entries(self, selected_dir: str, selected_type: str):
        for i in range(self._log_list.count()):
            item = self._log_list.item(i)
            widget = self._log_list.itemWidget(item)
            if isinstance(widget, LogEntryWidget):
                show = widget.matches_filter(selected_dir, selected_type)
                item.setHidden(not show)

    def _on_raw_data_toggled(self, checked: bool):
        self._update_widget_visibility(lambda w: w.set_raw_data_visible(checked))

    def _on_parsed_toggled(self, checked: bool):
        self._update_widget_visibility(lambda w: w.set_parsed_visible(checked))

    def _update_widget_visibility(self, update_fn):
        """Helper function to toggle something for all LogEntryWidgets and update the item height."""
        for i in range(self._log_list.count()):
            item = self._log_list.item(i)
            widget = self._log_list.itemWidget(item)
            if isinstance(widget, LogEntryWidget):
                update_fn(widget)
                item.setSizeHint(widget.sizeHint())

    def _on_auto_scroll_toggled(self, checked: bool):
        if checked:
            self._log_list.scrollToBottom()


class LogEntryWidget(QWidget):
    def __init__(
        self,
        dir: str,
        type: str,
        msg: str,
        raw_bytes: bytes | None = None,
        parsed: str = "",
    ):
        super().__init__()
        self._dir_type = dir.lower().strip()
        self._msg_type = type.lower().strip()
        self._raw_bytes = raw_bytes
        self._parsed = parsed

        self._create_layout(dir, type, msg, raw_bytes, parsed)
        self._setup_style()

    def _setup_style(self):
        bg_color = "#e5e7eb"
        col_text = "#000000"
        col_light_gray = "#d1d5dc"
        col_mid_gray = "#99a1af"

        style = f"""
        QFrame {{
            border: none;
            background-color: #F9FAFB;
        }}
        
        QFrame#info-div {{
            min-height: 14px;
        }}
        
        QLabel {{
            border: none;
            background-color: #F9FAFB;
        }}
        
        QLabel#dir-label {{
            font-size: 11px;
            padding: 2px 4px 0 4px;
            font-weight: 700;
            border: 2px solid {col_mid_gray};
            border-radius: 5px;
        }}

        QLabel#dir-label[dirType="sys"] {{
            background-color: #d1d5dc;
            color: {col_text};
        }}

        QLabel#dir-label[dirType="tx"] {{
            background-color: #BEDBFF;
            color: #193CB8;
        }}

        QLabel#dir-label[dirType="rx"] {{
            background-color: #B9F8CF;
            color: #016630;
        }}

        QLabel#dir-label[dirType="error"] {{
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

        QLabel#type-label[msgType="info"] {{
            background-color: {col_light_gray};
            color: {col_text};
        }}

        QLabel#type-label[msgType="data"] {{
            background-color: #BEDBFF;
            color: #193CB8;
        }}

        QLabel#type-label[msgType="error"] {{
            background-color: #FFC9C9;
            color: #9F0712;
        }}

        QLabel#type-label[msgType="warning"] {{
            background-color: #fff8d6;
            color: #ff7b00;
        }}

        QLabel#msg-label {{
            border: none;
            font-size: 12px;
            font-weight: 700;
            color: {col_text};
        }}

        QLabel#raw-data-label {{
            color: #193CB8;
            font-size: 12px;
            padding: 2px 2px;
            padding-top: 4px;
            border: 1px solid {col_light_gray};
        }}

        QLabel#parsed-label {{
            color: #016630;
            font-weight: 700;
            font-size: 12px;
            padding: 2px 2px;
            padding-top: 4px;
            border: 1px solid {col_light_gray};
        }}
        """
        self.setStyleSheet(style)

    def _create_layout(
        self, dir: str, type: str, msg: str, raw_bytes: bytes | None, parsed: str
    ):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(8, 8, 8, 8)

        # Info div with time, direction, and type
        info_div = QFrame()
        info_div.setObjectName("info-div")
        info_div_layout = QHBoxLayout(info_div)
        info_div_layout.setContentsMargins(0, 0, 0, 0)

        # Time label
        curr_t = QTime.currentTime().toString("hh:mm:ss.zzz AP")
        time_label = QLabel(f"{curr_t}")
        time_label.setStyleSheet("color: #343a40; font-size: 10px; font-weight: 700;")
        info_div_layout.addWidget(time_label)

        # Direction label
        self.dir_label = QLabel()
        self.dir_label.setObjectName("dir-label")
        self.dir_label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        valid_dirs = {"sys", "tx", "rx"}
        if self._dir_type in valid_dirs:
            self.dir_label.setProperty("dirType", self._dir_type)
            self.dir_label.setText(f"{dir.upper()}")
        else:
            self.dir_label.setProperty("dirType", "error")
            self.dir_label.setText("ERROR: Unknown dir label!")

        # Type label
        self.type_label = QLabel()
        self.type_label.setObjectName("type-label")
        self.type_label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        valid_types = {"data", "error", "info", "warning"}
        if self._msg_type in valid_types:
            self.type_label.setText(f"{type.upper()}")
            self.type_label.setProperty("msgType", self._msg_type)
        else:
            self.type_label.setProperty("msgType", "error")
            self.type_label.setText("ERROR: Unknown type label!")

        info_div_layout.addWidget(self.dir_label)
        info_div_layout.addWidget(self.type_label)
        info_div_layout.addStretch()

        # Message label
        self.msg_label = QLabel(f"{msg}")
        self.msg_label.setObjectName("msg-label")

        main_layout.addWidget(info_div)
        main_layout.addWidget(self.msg_label)

        # Raw data and parsed labels
        self.raw_data_label = None
        self.parsed_label = None

        if raw_bytes:
            self.raw_data_label = QLabel()
            self.raw_data_label.setObjectName("raw-data-label")
            try:
                raw_data_str = "Raw: " + " ".join(f"{b:02X}" for b in raw_bytes)
            except Exception as e:
                raw_data_str = f"Raw: <error: {e}>"
            self.raw_data_label.setText(raw_data_str)
            main_layout.addWidget(self.raw_data_label)

            self.parsed_label = QLabel()
            self.parsed_label.setText(f">> {parsed}")
            self.parsed_label.setObjectName("parsed-label")
            main_layout.addWidget(self.parsed_label)

    def set_raw_data_visible(self, visible: bool):
        if self.raw_data_label:
            self.raw_data_label.setVisible(visible)

    def set_parsed_visible(self, visible: bool):
        if self.parsed_label:
            self.parsed_label.setVisible(visible)

    def matches_filter(self, dir_filter: str, type_filter: str) -> bool:
        dir_match = dir_filter.lower() == "all" or self._dir_type == dir_filter.lower()
        type_match = (
            type_filter.lower() == "all" or self._msg_type == type_filter.lower()
        )
        return dir_match and type_match
