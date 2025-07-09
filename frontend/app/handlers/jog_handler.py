from PySide6.QtWidgets import QButtonGroup, QPushButton


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
        self._transl_btn_group = None
        self._rotation_btn_group = None

    def setup_connections(self):
        """Setup jog-related connections"""
        self._setup_jog_buttons()
        self._setup_cart_step_btns(0)
        self._setup_cart_step_btns(1)
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

        cart_jog_buttons = [
            self._ui.cart_x_pos_btn,
            self._ui.cart_x_neg_btn,
            self._ui.cart_y_pos_btn,
            self._ui.cart_y_neg_btn,
            self._ui.cart_z_pos_btn,
            self._ui.cart_z_neg_btn,
            self._ui.cart_rx_pos_btn,
            self._ui.cart_rx_neg_btn,
            self._ui.cart_ry_pos_btn,
            self._ui.cart_ry_neg_btn,
            self._ui.cart_rz_pos_btn,
            self._ui.cart_rz_neg_btn,
        ]
        for btn in cart_jog_buttons:
            btn.pressed.connect(lambda btn=btn: self._handle_cart_jog_btn_press(btn))
            btn.released.connect(self._handle_cart_jog_btn_release)

    def _setup_cart_step_btns(self, stp_mode):
        """0 for translation group
        1 for rotation group
        """
        if stp_mode == 0:
            container = self._ui.translation_btn_group
        elif stp_mode == 1:
            container = self._ui.rotation_btn_group
        btn_group = QButtonGroup()
        btn_group.setExclusive(True)

        for child in container.findChildren(QPushButton):
            if child.isCheckable():
                btn_group.addButton(child)
                if stp_mode == 0:
                    self._transl_btn_group = btn_group
                elif stp_mode == 1:
                    self._rotation_btn_group = btn_group

    def _handle_jog_btn_press(self, btn):
        """Handle jog button press"""
        joint, direction = self._parse_jog_button(btn)
        if joint and direction:
            self._jog_joint = joint
            self._jog_direction = direction
            speed = self._calculate_jog_speed(joint)
            start_data = f"JOG,[{joint}, {direction}, {speed}, START]"
            self._serial.send_data(start_data)

    def _handle_jog_btn_release(self):
        """Handle jog button release"""
        stop_data = f"JOG,[{self._jog_joint}, {self._jog_direction}, 0, STOP]"
        self._serial.send_data(stop_data)

    def _parse_jog_button(self, btn):
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

    def _handle_cart_jog_btn_press(self, btn):
        """Handles press on caresian jog btn.
        Collects data sring and forwards it to send_data function"""
        axis, direction = self._parse_jog_button(btn)
        direction = self._map_cart_direction(direction)
        mode = self._map_axis_to_mode(axis)
        transl_delta = self._get_transl_delta()
        rot_delta = self._get_rot_delta()
        start_cart_dta_str = self._create_cart_jog_data_str(
            axis, direction, mode, transl_delta, rot_delta
        )
        self._serial.send_data(start_cart_dta_str)

    def _create_cart_jog_data_str(self, axis, dir, mode, t_delta, r_delta):
        """Create data string for cartesian jog"""
        start_cart_data = f"JOG_CART,[{axis},"
        if mode == -1:
            print("ERROR: Failed in '_handle_cart_jog_btn_press'.")
        elif mode == "linear":
            speed_mm_s = self._helper.map_slider_to_linear_speed(
                self._slider_value, max_mm_s=30.0
            )
            # axis
            start_cart_data += f"LIN,{dir},{speed_mm_s},{t_delta}"
        else:
            speed_deg_s = self._helper.map_slider_to_rot_speed(
                self._slider_value, max_deg_s=30.0
            )
            start_cart_data += f"ROT,{dir},{speed_deg_s},{r_delta}"
        start_cart_data += ",START]"
        return start_cart_data

    def _map_cart_direction(self, dir):
        if dir == "POS":
            return 1
        return -1

    def _map_axis_to_mode(self, axis):
        linear_mode_axes = ["X", "Y", "Z"]
        rot_mode_axes = ["RX", "RY", "RZ"]
        if axis in linear_mode_axes:
            return "linear"
        elif axis in rot_mode_axes:
            return "rotation"
        return -1

    def _get_transl_delta(self):
        active_btn = self._transl_btn_group.checkedButton()
        if active_btn:
            return active_btn.text()[:-2]  # btnTxt = 0.1mm, 50mm

    def _get_rot_delta(self):
        active_btn = self._rotation_btn_group.checkedButton()
        if active_btn:
            return active_btn.text()[:-1]  # btnTxt = 1°, 30°

    def _handle_cart_jog_btn_release(self):
        pass

    def _handle_jog_slider_change(self):
        """Handle jog slider value change"""
        self._ui.jog_slider.setValue(round(self._ui.jog_slider.value() / 10) * 10)
        self._slider_value = self._ui.jog_slider.value()
