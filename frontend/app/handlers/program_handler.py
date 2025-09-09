from app.core.serial_connection import SerialConnection
from app.core.packet_builder import PacketBuilder
from app.ui.ui_manager import UIManager
from app.constants.com_protocol import (
    CMD_LOAD,
    NOP,
    CMD_START,
    CMD_STOP,
    CMD_IDLE,
    PRG_PING,
    PRG_HOME,
    PRG_TEST_SWITCHES,
)


from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from main import MainWindow


class ProgramHandler:
    def __init__(
        self, ui: "MainWindow", serial: SerialConnection, ui_manager: UIManager
    ):
        self._ui = ui
        self._serial = serial
        self._ui_manager = ui_manager
        self._pb = PacketBuilder()
        self._current_program: int | None = None

        self._ui_right_panel = self._ui.right_panel
        self._ui.robotActionTriggered.connect(self.handle_program_select)
        self._predefined_programs_map = {
            "ping": ("Ping", PRG_PING),
            "home": ("Home", PRG_HOME),
            "test_switches": ("Test Switches", PRG_TEST_SWITCHES),
        }

    def set_current_program(self, program: int | None):
        """Set current program"""
        self._current_program = program

    def handle_program_select(self, program: str):
        program_name: str | None = None
        if program in self._predefined_programs_map:
            program_name, self._current_program = self._predefined_programs_map[program]
        if self._serial.is_connected():
            if self._current_program is not None and program_name is not None:
                self._ui.btm_bar.program = program_name.upper()
                self._ui_right_panel.load_btn.setEnabled(True)

    def setup_connections(self):
        """Setup program-related connections"""
        self._ui_right_panel.run_btn.clicked.connect(self._handle_run_btn_click)
        self._ui_right_panel.stop_btn.clicked.connect(self._handle_stop_btn_click)
        self._ui_right_panel.load_btn.clicked.connect(self._handle_load_btn_click)

    def _handle_load_btn_click(self):
        """Handle load program button click"""
        if self._check_for_program():
            packet: bytes = self._pb.build_packet(
                cmd_id=CMD_LOAD, data=self._current_program
            )
            self._serial.set_data_out(packet, "Load selected program.")
            self._ui_right_panel.run_btn.setEnabled(True)
            self._ui_right_panel.stop_btn.setEnabled(False)

    def _handle_run_btn_click(self):
        """Handle start button click"""
        if self._check_for_program():
            packet: bytes = self._pb.build_packet(cmd_id=CMD_START, data=NOP)
            self._serial.set_data_out(packet, "Run selected program.")
            self._ui_right_panel.run_btn.setEnabled(False)
            self._ui_right_panel.stop_btn.setEnabled(True)

    def _handle_stop_btn_click(self):
        """Handle stop button click"""
        if self._check_for_program():
            packet: bytes = self._pb.build_packet(cmd_id=CMD_STOP, data=NOP)
            self._serial.set_data_out(packet, "Stop execution of selected program.")
            self._ui_right_panel.stop_btn.setEnabled(False)
            self._ui_right_panel.run_btn.setEnabled(True)

    def _handle_clear_btn_click(self):
        # self._ui_manager.log_message("[INFO]", "Program cleared", "lightblue")
        packet: bytes = self._pb.build_packet(cmd_id=CMD_IDLE, data=NOP)
        self._serial.set_data_out(packet, "Set execution state to idle.")
        self._current_program = None
        self._ui_right_panel.load_btn.setEnabled(False)
        self._ui_right_panel.run_btn.setEnabled(False)
        self._ui_right_panel.stop_btn.setEnabled(False)

    def _check_for_program(self):
        if self._current_program is not None:
            return True
        else:
            self._ui_manager.update_com_monitor("sys", "error", "No program loaded.")
            return False
