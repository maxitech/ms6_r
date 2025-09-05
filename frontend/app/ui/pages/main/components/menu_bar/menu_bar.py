from PySide6.QtGui import QAction
from PySide6.QtWidgets import QMainWindow, QMenuBar


class MenuBar(QMenuBar):
    def __init__(self, main_window: QMainWindow):
        super().__init__(main_window)
        self._main_window = main_window
        self._setup_style()
        self._create_bar()

    def _setup_style(self):
        bg_color = "#e5e7eb"
        col_text = "#000000"
        col_light_gray = "#d1d5dc"
        col_mid_gray = "#99a1af"

        style = f"""
            QMenuBar {{
                background-color: {bg_color};
                color: {col_text};
                border-bottom: 1px solid {col_mid_gray};
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
            
            QMenu {{
                background-color: {bg_color};
                color: {col_text};
            }}
            
            QMenu::item:selected, QMenu::item:pressed {{
                background-color: {col_light_gray};
            }}
            """
        self.setStyleSheet(style)

    def _create_bar(self):
        """Create application menu bar"""
        self._actions: list[QAction] = []

        # File menu
        file_menu = self.addMenu("File")

        new_program_action = QAction("New Program", self)
        new_program_action.setData("noop:new")
        self._actions.append(new_program_action)
        file_menu.addAction(new_program_action)

        open_program_action = QAction("Open Program", self)
        open_program_action.setData("noop:open")
        self._actions.append(open_program_action)
        file_menu.addAction(open_program_action)

        save_program_action = QAction("Save Program", self)
        save_program_action.setData("noop:save")
        self._actions.append(save_program_action)
        file_menu.addAction(save_program_action)

        save_as_action = QAction("Save As", self)
        save_as_action.setData("noop:save_as")
        self._actions.append(save_as_action)
        file_menu.addAction(save_as_action)

        file_menu.addSeparator()

        exit_action = QAction("Exit", self._main_window)
        exit_action.triggered.connect(self._main_window.close)
        file_menu.addAction(exit_action)

        # Edit menu
        edit_menu = self.addMenu("Edit")
        edit_menu.addAction("Undo")
        edit_menu.addAction("Redo")
        edit_menu.addSeparator()
        edit_menu.addAction("Cut")
        edit_menu.addAction("Copy")
        edit_menu.addAction("Paste")
        edit_menu.addSeparator()
        edit_menu.addAction("Find && Replace")

        # Robot menu
        robot_menu = self.addMenu("Robot")
        robot_menu.addAction("Calibrate Axes")
        robot_menu.addAction("Set Tool Frame")
        robot_menu.addAction("Set DH Parameters")
        robot_menu.addSeparator()

        # Predefined Programs submenu
        predefined_menu = robot_menu.addMenu("Predefined Programs")
        ping_action = QAction("Ping", self)
        predefined_menu.addAction(ping_action)

        test_switches_action = QAction("Test Switches", self)
        predefined_menu.addAction(test_switches_action)

        home_action = QAction("Home Robot", self)
        predefined_menu.addAction(home_action)

        view_menu = self.addMenu("View")
        view_main_page_act = QAction("Control Panel", self)
        view_main_page_act.setData("main:main_page")
        self._actions.append(view_main_page_act)
        view_menu.addAction(view_main_page_act)

        # Settings menu
        settings_menu = self.addMenu("Settings")
        com_settings_act = QAction("General", self)
        com_settings_act.setData("main:general_settings")
        self._actions.append(com_settings_act)
        settings_menu.addAction(com_settings_act)

        # Tools menu
        tools_menu = self.addMenu("Tools")
        editor_act = QAction("Editor", self)
        editor_act.setData("center:editor")
        self._actions.append(editor_act)
        tools_menu.addAction(editor_act)

        com_act = QAction("Communication Monitor", self)
        com_act.setData("center:com_monitor")
        self._actions.append(com_act)
        tools_menu.addAction(com_act)

        io_act = QAction("I/O Monitor", self)
        io_act.setData("center:io_monitor")
        self._actions.append(io_act)
        tools_menu.addAction(io_act)

        # tools_menu.addAction("Variable Monitor")
        # tools_menu.addAction("Diagnostics")
        # tools_menu.addAction("Log Viewer")
        # tools_menu.addAction("Backup/Restore")

        # Help menu
        help_menu = self.addMenu("Help")
        help_menu.addAction("User Manual")
        help_menu.addAction("Command Reference")

        about_act = QAction("About", self)
        about_act.setData("main:about_page")
        self._actions.append(about_act)
        help_menu.addAction(about_act)
