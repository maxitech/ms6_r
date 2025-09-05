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
from PySide6.QtCore import Qt


class ComMonitorPanel(QWidget):
    def __init__(self):
        super().__init__()
        self._setup_style()
        self._create_layout()

        self.add_log_entry("test")

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
            font-size: 12px;
            font-weight: 700;
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
        raw_data_check_box = QCheckBox("Raw Data")
        raw_data_check_box.setChecked(True)
        parsed_check_box = QCheckBox("Parsed")
        parsed_check_box.setChecked(True)
        self._auto_scroll_check_box = QCheckBox("Auto Scroll")
        self._auto_scroll_check_box.setChecked(True)
        self._auto_scroll_check_box.toggled.connect(self._on_auto_scroll_toggled)
        check_box_group_layout.addWidget(raw_data_check_box)
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
        frame.setFrameShape(QFrame.Shape.StyledPanel)
        frame.setStyleSheet(
            "background-color: #999; border: 1px solid #000; max-height: 70px;"
        )

        layout = QVBoxLayout(frame)
        label = QLabel(text)
        button = QPushButton("Action")
        layout.addWidget(label)
        layout.addWidget(button)

        self.content_layout.insertWidget(self.content_layout.count() - 1, frame)

    def ResizeScroll(self, _, max):
        if self._auto_scroll_check_box.isChecked():
            self.scroll_area.verticalScrollBar().setValue(max)

    def _on_auto_scroll_toggled(self, checked: bool):
        if checked:
            bar = self.scroll_area.verticalScrollBar()
            bar.setValue(bar.maximum())
