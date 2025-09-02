import os
import sys
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QGridLayout,
    QFrame,
)
from PySide6.QtGui import QFont

from app.controllers.main_window_controller import MainWindowController
from app.constants.com_protocol import CMD_IDLE, NOP
from app.core.packet_builder import PacketBuilder
from app.ui.components import (
    MenuBar,
    LeftPanel,
    RightPanel,
    EditorPanel,
    CentralBottomPanel,
    BottomBar,
)

os.environ["QT_STYLE_OVERRIDE"] = "Fusion"


def make_panel(name):
    panel = QFrame()
    panel.setFrameShape(QFrame.Shape.StyledPanel)
    panel.setObjectName(name)
    return panel


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        # self.ui = Ui_MainWindow()
        # self.ui.setupUi(self)
        # self.controller = MainWindowController(self.ui)
        self._pb = PacketBuilder()
        # ----------------------
        app_instance = QApplication.instance()
        if app_instance is not None:
            version = app_instance.applicationVersion()
            app_name = app_instance.applicationName()
        else:
            version = "unknown"
            app_name = "MS6_R Software Suite"
        self.setWindowTitle(f"{app_name} v{version} — by Maximilian Stadlmeyer")
        self.setMinimumSize(1200, 800)
        self.setMaximumSize(1920, 1080)
        self.setGeometry(100, 100, 1400, 900)

        # Init components
        self.setMenuBar(MenuBar(self))
        self._create_main_layout()

        # Setup UI
        self.setup_style()

    def setup_style(self):
        bg_color = "#e5e7eb"
        col_text = "#000000"
        col_text_disabled = "#6b7280"
        col_light_gray = "#d1d5dc"
        col_mid_gray = "#99a1af"
        col_hover_gray = "#bdc2cc"

        style = f"""
            QMainWindow {{
                background-color: {bg_color};
                font-family: 'MS Sans Serif', 'Liberation Sans', 'Arial', sans-serif;
                font-size: 11px;
                color: {col_text};
            }}
        
            QPushButton {{
                background-color: {col_light_gray};
                color: {col_text};
                border: 2px outset {col_mid_gray};
                border-radius: 0px;
                padding: 8px 0px;
            }}
            
            QPushButton:hover {{ 
                background-color: {col_hover_gray};
                color: {col_text};
            }}
            
            QPushButton::checked {{
                background-color: {col_mid_gray};
                color: {col_text_disabled};
            }}

            QPushButton::disabled {{
                background-color: {col_mid_gray};
                color: {col_text_disabled};
                border: 2px inset {col_mid_gray};
            }}
                        
            QFrame {{
                border: 1px solid {col_mid_gray};
                background-color: {bg_color};
            }}
            
            QFrame#left-panel QFrame, QFrame#right-panel QFrame {{
                background-color: {col_light_gray};
                border: 2px solid {col_mid_gray};
            }}
            
            QComboBox {{
                background-color: {bg_color};
                color: {col_text};
                min-height: 30px;
                padding-left: 8px;
            }}
            """
        self.setStyleSheet(style)

    def _create_main_layout(self):
        main_widget = QWidget()
        grid = QGridLayout(main_widget)
        grid.setContentsMargins(0, 0, 0, 0)
        grid.setSpacing(0)

        grid.addWidget(LeftPanel(), 0, 0, 2, 1)
        grid.addWidget(EditorPanel(), 0, 1, 1, 1)
        grid.addWidget(CentralBottomPanel(), 1, 1, 1, 1)
        grid.addWidget(RightPanel(), 0, 2, 2, 1)
        grid.addWidget(BottomBar(), 2, 0, 1, 3)

        self.setCentralWidget(main_widget)

    def closeEvent(self, event):
        print("Closing application...")
        packet: bytes = self._pb.build_packet(cmd_id=CMD_IDLE, data=NOP)
        # self.controller.set_data_out(packet)
        # self.controller.serial.disconnect()
        # TODO: Settings persistence
        event.accept()


def main():
    """Main entry point for the application."""
    app = QApplication(sys.argv)
    font = QFont("Courier New", 10)
    font.setLetterSpacing(QFont.SpacingType.PercentageSpacing, 95)
    app.setFont(font)

    # Set application properties
    app.setApplicationName("MS6_R Software Suite")
    app.setApplicationVersion("1.0.0")
    app.setOrganizationName("Stadlmeyer Robotics")

    # Create and show main window
    window = MainWindow()
    window.show()
    return app.exec()


if __name__ == "__main__":
    sys.exit(main())
