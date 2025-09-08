from PySide6.QtCore import QTimer
from app.core.serial_connection import SerialConnection
from app.core.setup import Setup
from app.utils.helper import Helper
from app.ui.ui_manager import UIManager
from app.handlers.connection_handler import ConnectionHandler

from app.handlers.jog_handler import JogHandler
from app.handlers.program_handler import ProgramHandler
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from main import MainWindow


class MainWindowController:
    def __init__(self, ui: "MainWindow"):
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
        self._program_handler = ProgramHandler(ui, self.serial, self._ui_manager)
        self._jog_handler = JogHandler(ui, self.serial, self._helper, self._ui_manager)

        self._setup_connections()
        self._setup_timers()

    def _setup_connections(self):
        """Setup all UI connections"""
        self._connection_handler.setup_connections()
        self._jog_handler.setup_connections()
        self._program_handler.setup_connections()

        # Setup specific connections
        # self._ui.setup_save_btn.clicked.connect(self._handle_setup_save)

    def _setup_timers(self):
        """Setup all timers"""
        self._port_timer = QTimer()
        self._port_timer.timeout.connect(self._connection_handler.check_ports)
        self._port_timer.timeout.connect(self._connection_handler.check_status)
        self._connection_handler.connection_changed.connect(
            self._update_timer_intervall
        )
        self._update_timer_intervall()

    def _update_timer_intervall(self, connected: bool = False):
        interval = 250 if not connected else 500
        self._port_timer.start(interval)

    def set_data_out(self, packet):
        """Send data through serial connection"""
        self.serial.set_data_out(packet)

    def _handle_setup_save(self):
        """Handle setup save"""
        self._setup.save()
