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
        # File menu
        file_menu = self.addMenu("File")

        new_action = QAction("New Program", self)
        new_action.triggered.connect(self._new_program)
        file_menu.addAction(new_action)

        open_action = QAction("Open Program", self)
        open_action.triggered.connect(self._open_program)
        file_menu.addAction(open_action)

        save_action = QAction("Save Program", self)
        save_action.triggered.connect(self._save_program)
        file_menu.addAction(save_action)

        save_as_action = QAction("Save As...", self)
        save_as_action.triggered.connect(self._save_program_as)
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
        edit_menu.addAction("Find & Replace")

        # Robot menu
        robot_menu = self.addMenu("Robot")
        robot_menu.addAction("Calibrate Axes")
        robot_menu.addAction("Set Tool Frame")
        robot_menu.addAction("Set Base Frame")
        robot_menu.addAction("Safety Configuration")

        # Settings menu
        settings_menu = self.addMenu("Settings")
        settings_menu.addAction("Communication Settings")
        settings_menu.addAction("Motion Parameters")
        settings_menu.addAction("User Preferences")
        settings_menu.addAction("Language")
        # settings_menu.addAction("Import/Export Settings")

        # Tools menu
        tools_menu = self.addMenu("Tools")
        tools_menu.addAction("I/O Monitor")
        tools_menu.addAction("Variable Monitor")
        tools_menu.addAction("Diagnostics")
        tools_menu.addAction("Log Viewer")
        tools_menu.addAction("Backup/Restore")

        # Help menu
        help_menu = self.addMenu("Help")
        help_menu.addAction("User Manual")
        help_menu.addAction("Command Reference")
        help_menu.addAction("About")

    # Menu action implementations
    def _new_program(self):
        """Create new program"""
        # self.program_editor.new_program()
        pass

    def _open_program(self):
        """Open existing program file"""
        # self.program_editor.open_program()
        pass

    def _save_program(self):
        """Save current program"""
        # self.program_editor.save_program()
        pass

    def _save_program_as(self):
        """Save program with new name"""
        # self.program_editor.save_program_as()
        pass
