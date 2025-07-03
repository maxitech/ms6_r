from app.constants.commands import COMMAND_MAP


class CommandHandler:
    def __init__(self, ui, ui_manager):
        self._ui = ui
        self._ui_manager = ui_manager
        self._command_map = COMMAND_MAP

    def setup_connections(self):
        """Setup command button connections"""
        for btn_name in self._command_map:
            btn = getattr(self._ui, btn_name)
            btn.clicked.connect(lambda _, b=btn: self._handle_cmd_btn_click(b))

    def _handle_cmd_btn_click(self, clicked_button):
        """Handle command button click"""
        btn_name = clicked_button.objectName()
        if btn_name in self._command_map:
            name, command = self._command_map[btn_name]
            self._ui_manager.update_program_monitor(name, "lightblue", "white")
            self._ui_manager.set_current_program(command)
            self._ui.btn_load_prog_btn.setEnabled(True)
