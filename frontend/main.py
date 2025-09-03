import os
import sys
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QGridLayout,
    QFrame,
    QStackedWidget,
    QVBoxLayout,
    QLabel,
)
from PySide6.QtGui import QFont, QAction

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

        # top-level stacks
        self.main_stack = QStackedWidget(self)  # full-window pages
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
        """
        Build two-level stacking:
         - self.main_stack: holds full-window pages (main app, settings, about)
         - inside the "Main App" page we create a center_stack that swaps the central tool area
        """
        main_app_page = QWidget()
        app_grid = QGridLayout(main_app_page)
        app_grid.setContentsMargins(0, 0, 0, 0)
        app_grid.setSpacing(0)

        self.left_panel = LeftPanel()
        self.right_panel = RightPanel()

        # center container: top = center_stack(editor / io moitor / etc), bottom = CentralBottomPanel
        center_container = QFrame()
        center_layout = QVBoxLayout(center_container)
        center_layout.setContentsMargins(0, 0, 0, 0)
        center_layout.setSpacing(0)

        # inner stack for central tools
        self.center_stack = QStackedWidget()

        editor_page = EditorPanel()
        # ! temp
        io_monitor_page = QWidget()
        io_layout = QVBoxLayout(io_monitor_page)
        io_layout.addWidget(QLabel("I/O Monitor (placeholder)"))

        # add pages to stack
        self.center_stack.addWidget(editor_page)
        self.center_stack.addWidget(io_monitor_page)

        center_layout.addWidget(self.center_stack)
        center_layout.addWidget(CentralBottomPanel())

        app_grid.addWidget(self.left_panel, 0, 0, 2, 1)
        app_grid.addWidget(center_container, 0, 1, 2, 1)
        app_grid.addWidget(self.right_panel, 0, 2, 2, 1)
        app_grid.addWidget(BottomBar(), 2, 0, 1, 3)

        # Build other full-window pages
        general_settings_page = QWidget()
        s_layout = QVBoxLayout(general_settings_page)
        s_layout.addWidget(QLabel("General Settings Page (full-window)"))

        about_page = QWidget()
        a_layout = QVBoxLayout(about_page)
        a_layout.addWidget(QLabel("About Page (full-window)"))

        # Add pages to main_stack and set initial page to the app
        self.main_stack.addWidget(main_app_page)
        self.main_stack.addWidget(general_settings_page)
        self.main_stack.addWidget(about_page)
        self.main_stack.setCurrentIndex(0)

        # Make the main_stack the central widget of the QMainWindow
        self.setCentralWidget(self.main_stack)

        mb = self.menuBar()
        for act in getattr(mb, "_actions", []):
            act.triggered.connect(self._on_menu_action_triggered)

    def _on_menu_action_triggered(self):
        sender = self.sender()
        if not isinstance(sender, QAction):
            return
        data = sender.data() or ""
        # format: "center:io_monitor" or "main:settings" or "noop:save"
        if isinstance(data, str) and ":" in data:
            scope, key = data.split(":", 1)
            if scope == "center":
                self._show_center_page(key)
            elif scope == "main":
                self._show_main_page(key)
            elif scope == "noop":
                print("noop action:", key)
        else:
            # fallback: handle unknown actions if needed(later)
            pass

    def _show_main_page(self, page: str):
        mapping = {"main_page": 0, "general_settings": 1, "about_page": 2}
        idx = mapping.get(page)
        if idx is None:
            return
        self.main_stack.setCurrentIndex(idx)

    def _show_center_page(self, page: str):
        mapping = {"editor": 0, "io_monitor": 1}
        idx = mapping.get(page)
        if idx is None:
            return
        self.center_stack.setCurrentIndex(idx)

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
