from app.constants.jog_config import JOG_CONFIG


class JogHandler:
    def __init__(self, ui, serial, helper, ui_manager):
        self._ui = ui
        self._serial = serial
        self._helper = helper
        self._ui_manager = ui_manager
        self._pb = PacketBuilder()
        self._slider_value = ui.jog_slider.value()
        self._jog_timer = QTimer()
        self._jog_timer.setInterval(10)
        self._jog_timer.timeout.connect(self._queue_jog_cmd)
        self._serial_packet: bytes | None = None

    def setup_connections(self):
        """Setup jog-related connections"""
        self._setup_jog_buttons()
        self._ui.jog_slider.valueChanged.connect(self._handle_jog_slider_change)

    def _setup_jog_buttons(self):
        """Setup all jog button connections"""
        jog_buttons = [
            self._ui.jog_j1_pos_btn,
            self._ui.jog_j1_neg_btn,
            self._ui.jog_j2_pos_btn,
            self._ui.jog_j2_neg_btn,
            self._ui.jog_j3_pos_btn,
            self._ui.jog_j3_neg_btn,
            self._ui.jog_j4_pos_btn,
            self._ui.jog_j4_neg_btn,
            self._ui.jog_j5_pos_btn,
            self._ui.jog_j5_neg_btn,
            self._ui.jog_j6_pos_btn,
            self._ui.jog_j6_neg_btn,
        ]

        for button in jog_buttons:
            button.pressed.connect(lambda btn=button: self._handle_jog_btn_press(btn))
            button.released.connect(self._handle_jog_btn_release)

    def _handle_jog_btn_press(self, btn):
        """Handle jog button press"""
        joint_i, direction = self._parse_jog_button(btn)
        if joint_i is not None and direction:
            self._create_jog_cmd(i=joint_i, dir=direction)
            self._jog_timer.start()

    def _create_jog_cmd(self, i: int, dir: int):
        jog_speeds = [0, 0, 0, 0, 0, 0]  # repr: speed for each joint
        speed = self._calculate_jog_speed(i)
        jog_speeds[i] = speed * dir
        self._serial_packet = self._pb.build_packet(cmd_id=CMD_JOG, data=jog_speeds)

    def _queue_jog_cmd(self):
        self._serial.set_data_out(self._serial_packet)

    def _handle_jog_btn_release(self):
        """Handle jog button release"""
        self._jog_timer.stop()
        shared_data.clear_data_queue_out()

    def _parse_jog_button(self, btn) -> Tuple[int | None, int | None]:
        """Parse jog button name to get joint and direction"""
        btn_name = btn.objectName()
        parts = btn_name.split("_")
        if len(parts) == 4:
            joint: str = parts[1].upper()  # J1-J6
            dir: str = parts[2].upper()  # POS || NEG
            joint_i = int(joint[-1]) - 1
            if dir == "POS":
                dir: int = 1
            else:
                dir: int = -1
            return joint_i, dir

        return None, None

    def _calculate_jog_speed(self, joint):
        """Calculate jog speed based on joint and slider value"""
        gear_factors = JOG_CONFIG["gear_factors"]
        joint_speeds = self._helper.get_joint_speeds(
            slider_value=self._slider_value,
            v_min=JOG_CONFIG["speed_range"]["min"],
            v_max=JOG_CONFIG["speed_range"]["max"],
            gear_factors=gear_factors,
        )
        index = int(joint.split("J")[1]) - 1
        return joint_speeds[index]

    def _handle_jog_slider_change(self):
        """Handle jog slider value change"""
        self._ui.jog_slider.setValue(round(self._ui.jog_slider.value() / 10) * 10)
        self._slider_value = self._ui.jog_slider.value()
