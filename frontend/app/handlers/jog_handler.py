from app.constants.jog_config import JOG_CONFIG


class JogHandler:
    def __init__(self, ui, serial, helper, ui_manager):
        self._ui = ui
        self._serial = serial
        self._helper = helper
        self._ui_manager = ui_manager
        self._jog_joint = ""
        self._jog_direction = ""
        self._slider_value = ui.jog_slider.value()

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
        joint, direction = self._parse_jog_button(btn)
        if joint and direction:
            self._jog_joint = joint
            self._jog_direction = direction
            speed = self._calculate_jog_speed(joint)
            start_data = f"JOG,[{joint}, {direction}, {speed}, START]"
            self._serial.set_data_out(start_data)

    def _handle_jog_btn_release(self):
        """Handle jog button release"""
        stop_data = f"JOG,[{self._jog_joint}, {self._jog_direction}, 0, STOP]"
        self._serial.set_data_out(stop_data)

    def _parse_jog_button(self, btn) -> Tuple[int | None, int | None]:
        """Parse jog button name to get joint and direction"""
        btn_name = btn.objectName()
        parts = btn_name.split("_")
        if len(parts) == 4:
            return parts[1].upper(), parts[2].upper()
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
