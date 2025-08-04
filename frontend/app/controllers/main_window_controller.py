from PySide6.QtCore import QTimer
from app.core.serial_connection import SerialConnection
from app.core.setup import Setup
from app.utils.helper import Helper
from app.ui.ui_manager import UIManager
from app.handlers.connection_handler import ConnectionHandler
from app.handlers.command_handler import CommandHandler
from app.handlers.jog_handler import JogHandler
from app.handlers.program_handler import ProgramHandler


class MainWindowController:
    def __init__(self, ui):
        # Note: Do not change order of init!
        self._ui = ui
        self._setup = Setup(ui=self._ui)
        self._helper = Helper()
        self._ui_manager = UIManager(ui)
        self.serial: SerialConnection = SerialConnection(self._helper, self._ui_manager)
        self._connection_handler = ConnectionHandler(
            ui, self._setup, self.serial, self._ui_manager
        )
        self.serial.set_connection_handler(self._connection_handler)
        self._command_handler = CommandHandler(ui, self._ui_manager)
        self._program_handler = ProgramHandler(ui, self.serial, self._ui_manager)
        self._jog_handler = JogHandler(ui, self.serial, self._helper, self._ui_manager)

        self._setup_connections()
        self._setup_timers()

    def _setup_connections(self):
        """Setup all UI connections"""
        self._connection_handler.setup_connections()
        self._command_handler.setup_connections()
        self._jog_handler.setup_connections()
        self._program_handler.setup_connections()

        # Setup specific connections
        self._ui.setup_save_btn.clicked.connect(self._handle_setup_save)

    def _setup_timers(self):
        """Setup all timers"""
        self._port_timer = QTimer()
        self._port_timer.timeout.connect(self._connection_handler.check_ports)
        self._port_timer.timeout.connect(self._connection_handler.check_status)
        self._port_timer.start(1000)

    def set_data_out(self, data):
        """Send data through serial connection"""
        self.serial.set_data_out(data)

    def _handle_setup_save(self):
        """Handle setup save"""
        self._setup.save()
