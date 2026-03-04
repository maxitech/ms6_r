from PySide6.QtWidgets import QPushButton
from typing import TYPE_CHECKING

from typing import Tuple
from app.core.setup import Setup
from app.core.serial_connection import SerialConnection
from app.core.packet_builder import packet_builder
from app.constants.ms6_r_constants import MS6_R_CONSTANTS as RC
from app.constants.com_protocol import CMD_JOG, CMD_MOVE_TO_POS
from app.core.shared.shared_data import shared_data

if TYPE_CHECKING:
    from main import MainWindow


class MotionHandler:
    def __init__(self, ui: "MainWindow", serial: SerialConnection, helper, ui_manager):
        self._ui = ui
        self._serial = serial
        self._helper = helper
        self._ui_manager = ui_manager
        self._setup = Setup(ui_manager=self._ui_manager, ui=self._ui)
        self._slider_value = ui.left_panel.ctrl.jog_speed_slider_val
        self._serial_packet: bytes | None = None

    def setup_connections(self):
        """Setup program-related connections"""
        self._move_home()
        self._ui.central_btm_panel.btn_teach_pos.clicked.connect(
            self._handle_teach_pos_btn_click
        )
        self._ui.central_btm_panel.btn_run_seq.clicked.connect(
            self._handle_run_seq_btn_click
        )
        self._ui.central_btm_panel.btn_del_last.clicked.connect(
            self._handle_del_last_btn_click
        )

    def _move_home(self):
        btn = self._ui.left_panel.ctrl.btn_home_pos
        btn.clicked.connect(self._handle_home_btn_click)

    def _handle_home_btn_click(self):
        target = [40250, 42760, -11820, -24080, -16438, -6400]
        packet = packet_builder.build_packet(CMD_MOVE_TO_POS, target)
        shared_data.set_data_out(packet)

    def _handle_teach_pos_btn_click(self):
        pos = shared_data.get_steps()
        if not pos:
            print("No position data available to teach.")
            return
        shared_data.add_position_to_sequence(pos)
        print(shared_data.get_sequence())

    def _handle_run_seq_btn_click(self):
        is_running = shared_data.is_run_sequence()
        if is_running:
            print("Already running sequence, wait for it to finish!")
            return
        else:
            if not shared_data.get_sequence():
                print("No sequence available to run.")
                return
            print("Starting sequence")
            shared_data.set_is_run_sequence(True)

    def _handle_del_last_btn_click(self):
        shared_data.del_position_from_sequence()
        print(shared_data.get_sequence())
