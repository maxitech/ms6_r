from app.core.packet_builder import PacketBuilder
from app.constants.com_protocol import CMD_LOAD, NOP, CMD_START, CMD_STOP, CMD_IDLE


class ProgramHandler:
    def __init__(self, ui, serial, ui_manager):
        self._ui = ui
        self._serial = serial
        self._ui_manager = ui_manager
        self._pb = PacketBuilder()
        self._current_program: int | None = None

    def setup_connections(self):
        """Setup program-related connections"""
        self._ui.btn_start.clicked.connect(self._handle_start_btn_click)
        self._ui.btn_stop.clicked.connect(self._handle_stop_btn_click)
        self._ui.btn_load_prog_btn.clicked.connect(self._handle_load_btn_click)
        self._ui.prog_clear_btn.clicked.connect(self._handle_clear_btn_click)

    def _handle_load_btn_click(self):
        """Handle load program button click"""
        self._current_program: int | None = self._ui_manager.get_current_program()

        if self._check_for_program():
            packet: bytes = self._pb.build_packet(
                cmd_id=CMD_LOAD, data=self._current_program
            )
            self._serial.set_data_out(packet)
            self._ui.btn_start.setEnabled(True)
            self._ui.btn_stop.setEnabled(False)

    def _handle_start_btn_click(self):
        """Handle start button click"""
        if self._check_for_program():
            packet: bytes = self._pb.build_packet(cmd_id=CMD_START, data=NOP)
            self._serial.set_data_out(packet)
            self._ui.btn_start.setEnabled(False)
            self._ui.btn_stop.setEnabled(True)

    def _handle_stop_btn_click(self):
        """Handle stop button click"""
        if self._check_for_program():
            packet: bytes = self._pb.build_packet(cmd_id=CMD_STOP, data=NOP)
            self._serial.set_data_out(packet)
            self._ui.btn_stop.setEnabled(False)
            self._ui.btn_start.setEnabled(True)

    def _handle_clear_btn_click(self):
        self._ui_manager.log_message("[INFO]", "Program cleared", "lightblue")
        packet: bytes = self._pb.build_packet(cmd_id=CMD_IDLE, data=NOP)
        self._serial.set_data_out(packet)
        self._current_program = None
        self._ui.btn_load_prog_btn.setEnabled(False)
        self._ui.btn_start.setEnabled(False)
        self._ui.btn_stop.setEnabled(False)

    def _check_for_program(self):
        if self._current_program is not None:
            return True
        else:
            self._ui_manager.log_message("[ERROR]", "No program loaded!", "red")
            return False
