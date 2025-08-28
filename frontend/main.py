import os
import sys
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QSplitter,
    QMenuBar,
    QStatusBar,
    QMessageBox,
)
from PySide6.QtGui import QFont, QAction
from app.ui_gen.main_window_ui import Ui_MainWindow
from app.controllers.main_window_controller import MainWindowController
from app.constants.com_protocol import CMD_IDLE, NOP
from app.core.packet_builder import PacketBuilder


os.environ["QT_STYLE_OVERRIDE"] = "Fusion"


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        # self.ui = Ui_MainWindow()
        # self.ui.setupUi(self)
        # self.controller = MainWindowController(self.ui)
        self._pb = PacketBuilder()
        # ----------------------
        self.setWindowTitle("MS6_R by Maximilian Stadlmeyer")
        self.setGeometry(100, 100, 1400, 900)

        # Init components
        self.create_menu_bar()

        # Setup UI
        self.setup_style()

    def setup_style(self):
        """Apply Windows 95/98 industrial style"""
        bg_color = "#e5e7eb"
        col_text = "#000000"
        col_light_gray = "#d1d5dc"
        col_mid_gray = "#99a1af"

        style = f"""
            QMainWindow {{
                background-color: {bg_color};
                font-family: 'MS Sans Serif', 'Liberation Sans', 'Arial', sans-serif;
                font-size: 11px;
                color: {col_text};
            }}

            QPushButton {{
                background-color: {bg_color};
                border: 2px outset #c0c0c0;
                padding: 4px 8px;
                min-height: 20px;
                font-weight: normal;
            }}

            QPushButton:pressed {{
                border: 2px inset #c0c0c0;
            }}

            QPushButton:disabled {{
                background-color: #808080;
                color: #606060;
                border: 2px outset #808080;
            }}

            QPushButton.emergency {{
                background-color: #ff4444;
                color: white;
                font-weight: bold;
                border: 2px outset #ff4444;
            }}

            QPushButton.emergency:pressed {{
                background-color: #cc0000;
                border: 2px inset #cc0000;
            }}
            
            QPushButton.emergency:checked {{
                background-color: #cc0000;
                border: 2px inset #cc0000;
            }}
            
            QFrame {{
                border: 2px inset #c0c0c0;
                background-color: #c0c0c0;
            }}
            
            QGroupBox {{
                border: 2px inset #c0c0c0;
                font-weight: bold;
                padding-top: 10px;
                margin-top: 6px;
            }}
            
            QGroupBox::title {{
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px 0 5px;
            }}

            QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {{
                border: 2px inset #c0c0c0;
                background-color: white;
                padding: 2px;
            }}

            QTextEdit, QPlainTextEdit {{
                border: 2px inset #c0c0c0;
                background-color: white;
                font-family: 'Courier New', monospace;
            }}

            QMenuBar {{
                background-color: {bg_color};
                border-bottom: 2px solid {col_mid_gray};
                min-height: 24px;
                spacing: 0;
            }}

            QMenuBar::item {{
                padding: 0px 4px;
                border:none;
            }}

            QMenuBar::item:selected, QMenuBar::item:pressed {{
                background-color: {col_light_gray};
            }}

            QStatusBar {{
                background-color: #c0c0c0;
                border-top: 1px solid #808080;
            }}

            .status-indicator {{
                border: 2px inset #c0c0c0;
                border-radius: 8px;
            }}

            .connected {{
                background-color: #00ff00;
            }}

            .disconnected {{
                background-color: #ff0000;
            }}
            """
        self.setStyleSheet(style)

    def create_menu_bar(self):
        """Create application menu bar"""
        menubar = self.menuBar()
        menubar.setContentsMargins(0, 0, 0, 0)

        # File menu
        file_menu = menubar.addMenu("File")

        new_action = QAction("New Program", self)
        new_action.triggered.connect(self.new_program)
        file_menu.addAction(new_action)

        open_action = QAction("Open Program", self)
        open_action.triggered.connect(self.open_program)
        file_menu.addAction(open_action)

        save_action = QAction("Save Program", self)
        save_action.triggered.connect(self.save_program)
        file_menu.addAction(save_action)

        save_as_action = QAction("Save As...", self)
        save_as_action.triggered.connect(self.save_program_as)
        file_menu.addAction(save_as_action)

        file_menu.addSeparator()

        # Predefined Programs submenu
        predefined_menu = file_menu.addMenu("Predefined Programs")

        ping_action = QAction("Ping", self)
        # ping_action.triggered.connect(lambda: self.load_predefined_program("ping"))
        predefined_menu.addAction(ping_action)

        test_switches_action = QAction("Test Switches", self)
        # test_switches_action.triggered.connect(
        #     lambda: self.load_predefined_program("test_switches")
        # )
        predefined_menu.addAction(test_switches_action)

        home_action = QAction("Home Robot", self)
        # home_action.triggered.connect(lambda: self.load_predefined_program("home"))
        predefined_menu.addAction(home_action)

        # predefined_menu.addSeparator()
        # predefined_manager_action = QAction("Manage Predefined Programs...", self)
        # predefined_manager_action.triggered.connect(
        #     self.show_predefined_programs_dialog
        # )
        # predefined_menu.addAction(predefined_manager_action)

        file_menu.addSeparator()

        exit_action = QAction("Exit", self)
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)

        # Edit menu
        edit_menu = menubar.addMenu("Edit")
        edit_menu.addAction("Undo")
        edit_menu.addAction("Redo")
        edit_menu.addSeparator()
        edit_menu.addAction("Cut")
        edit_menu.addAction("Copy")
        edit_menu.addAction("Paste")
        edit_menu.addSeparator()
        edit_menu.addAction("Find & Replace")

        # Robot menu
        robot_menu = menubar.addMenu("Robot")
        robot_menu.addAction("Calibrate Axes")
        robot_menu.addAction("Set Tool Frame")
        robot_menu.addAction("Set Base Frame")
        robot_menu.addAction("Safety Configuration")

        # Settings menu
        settings_menu = menubar.addMenu("Settings")
        settings_menu.addAction("Communication Settings")
        settings_menu.addAction("Motion Parameters")
        settings_menu.addAction("User Preferences")
        settings_menu.addAction("Language")
        # settings_menu.addAction("Import/Export Settings")

        # Tools menu
        tools_menu = menubar.addMenu("Tools")
        tools_menu.addAction("I/O Monitor")
        tools_menu.addAction("Variable Monitor")
        tools_menu.addAction("Diagnostics")
        tools_menu.addAction("Log Viewer")
        tools_menu.addAction("Backup/Restore")

        # Help menu
        help_menu = menubar.addMenu("Help")
        help_menu.addAction("User Manual")
        help_menu.addAction("Command Reference")
        help_menu.addAction("About")

    # Menu action implementations
    def new_program(self):
        """Create new program"""
        # self.program_editor.new_program()
        pass

    def open_program(self):
        """Open existing program file"""
        # self.program_editor.open_program()
        pass

    def save_program(self):
        """Save current program"""
        # self.program_editor.save_program()
        pass

    def save_program_as(self):
        """Save program with new name"""
        # self.program_editor.save_program_as()
        pass

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

    # Set application properties
    app.setApplicationName("MS6_R")
    app.setApplicationVersion("1.0.0")
    app.setOrganizationName("Empty for now")

    # Create and show main window
    window = MainWindow()
    window.show()
    return app.exec()


if __name__ == "__main__":
    sys.exit(main())
