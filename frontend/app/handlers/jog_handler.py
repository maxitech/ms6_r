from PySide6.QtWidgets import QPushButton
from typing import TYPE_CHECKING

from typing import Tuple
from app.core.serial_connection import SerialConnection
from app.core.packet_builder import PacketBuilder
from app.constants.ms6_r_constants import MS6_R_CONSTANTS as RC
from app.constants.com_protocol import CMD_JOG

if TYPE_CHECKING:
    from main import MainWindow


class JogHandler:
    def __init__(self, ui: "MainWindow", serial: SerialConnection, helper, ui_manager):
        self._ui = ui
        self._serial = serial
        self._helper = helper
        self._ui_manager = ui_manager
        self._pb = PacketBuilder()
        self._slider_value = ui.left_panel.ctrl.jog_speed_slider_val
        self._serial_packet: bytes | None = None

    def setup_connections(self):
        """Setup jog-related connections"""
        self._setup_jog_buttons()
        self._ui.left_panel.ctrl.jog_speed_slider.valueChanged.connect(
            self._handle_jog_slider_change
        )

    def _setup_jog_buttons(self):
        """Setup all jog button connections"""
        for i in range(self._ui.left_panel.ctrl.button_stack.count()):
            frame = self._ui.left_panel.ctrl.button_stack.widget(i)
            buttons = frame.findChildren(QPushButton)
            for button in buttons:
                button.pressed.connect(
                    lambda btn=button: self._handle_jog_btn_press(btn)
                )
                button.released.connect(self._handle_jog_btn_release)

    def _handle_jog_btn_press(self, btn: QPushButton):
        """Handle jog button press"""
        if "cart" in btn.objectName():
            print("handle-jog-cart-btn-click")
        else:
            joint_i, direction = self._parse_jog_button(btn)
            if joint_i is not None and direction:
                self._create_jog_cmd(i=joint_i, dir=direction)
                self._queue_jog_cmd()

    def _create_jog_cmd(self, i: int, dir: int):
        self._jog_speeds = [0, 0, 0, 0, 0, 0]  # repr: speed for each joint
        speed = self._calculate_jog_speed(i)
        self._jog_speeds[i] = speed * dir
        self._serial_packet = self._pb.build_packet(
            cmd_id=CMD_JOG, data=self._jog_speeds
        )

    def _queue_jog_cmd(self):
        if self._serial_packet:
            self._serial.set_data_out(self._serial_packet, str(self._jog_speeds))

    def _handle_jog_btn_release(self):
        """Handle jog button release"""
        self._jog_speeds = [0, 0, 0, 0, 0, 0]  # repr: speed for each joint
        self._serial_packet = self._pb.build_packet(
            cmd_id=CMD_JOG, data=self._jog_speeds
        )
        self._queue_jog_cmd()
        # shared_data.clear_data_queue_out()

    def _parse_jog_button(self, btn: QPushButton) -> Tuple[int | None, int | None]:
        """Parse jog button name to get joint and direction"""
        btn_name = btn.objectName()
        parts = btn_name.split("-")
        if len(parts) == 4:
            joint: str = parts[2].upper()  # J1-J6
            dir_str: str = parts[3].upper()  # POS || NEG
            joint_i = int(joint[-1]) - 1
            dir: int
            if dir_str == "POS":
                dir = 1
            else:
                dir = -1
            return joint_i, dir

        return None, None

    def _calculate_jog_speed(self, i: int):
        """Calculate jog speed based on joint and slider value"""
        gear_factors = RC.GEARBOX_RATIO
        joint_speeds = self._helper.get_joint_speeds(
            slider_value=self._slider_value,
            s_min=RC.JOINT_JOG_SPEED_RANGE[0],
            s_max=RC.JOINT_JOG_SPEED_RANGE[1],
            gear_factors=gear_factors,
        )
        return joint_speeds[i]

    def _handle_jog_slider_change(self):
        """Handle jog slider value change"""
        raw_val = self._ui.left_panel.ctrl.jog_speed_slider_val
        rounded_val = int(round(raw_val / 10) * 10)
        self._ui.left_panel.ctrl.jog_speed_slider_val = rounded_val
        self._ui_manager.update_slider_label(rounded_val)
        self._slider_value = rounded_val
