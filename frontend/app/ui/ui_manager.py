import datetime
import re

from app.robotics.robot import Robot
from app.constants.ms6_r_constants import MS6_R_CONSTANTS

from PySide6.QtWidgets import QPushButton
from PySide6.QtGui import QTextCharFormat, QColor, QTextCursor
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from main import MainWindow


class UIManager:
    def __init__(self, ui: "MainWindow"):
        self._ui = ui
        self.R = Robot(constants=MS6_R_CONSTANTS)
        self._current_program = None

        self._ui.left_panel.ctrl.btn_radio1.toggled.connect(
            lambda checked: self._handle_radio_toggle("JOINT") if checked else None
        )
        self._ui.left_panel.ctrl.btn_radio2.toggled.connect(
            lambda checked: self._handle_radio_toggle("CART") if checked else None
        )

    def _handle_radio_toggle(self, mode: str):
        if not isinstance(mode, str):
            return
        mode = mode.upper()
        self._ui.btm_bar.mode = self._ui.btm_bar.mode = mode

    def update_combo_box(self, ports):
        """Update port combo box"""
        combo_box = self._ui.left_panel.con.combo_box
        combo_box.clear()
        combo_box.addItems(ports)

    def update_ui_based_on_connection_status(
        self, btn_text, status_text, is_enabled, port
    ):
        """Update UI based on connection status"""
        con_btn = self._ui.left_panel.con.con_btn
        combo_box = self._ui.left_panel.con.combo_box
        con_status_label = self._ui.left_panel.con.con_status_label

        con_btn.setText(btn_text)
        combo_box.setEnabled(is_enabled)
        con_status_label.setText(status_text)
        self._ui.btm_bar.con_status = status_text
        self._ui.btm_bar.com_port = port

        # Toggle Enabled/Disabled
        left_panel_ctrl = self._ui.left_panel.ctrl
        left_panel_con = self._ui.left_panel.con
        central_btm_panel = self._ui.central_btm_panel

        left_panel_con.emergency_stop_btn.setEnabled(not is_enabled)

        left_panel_ctrl.btn_radio1.setEnabled(not is_enabled)
        left_panel_ctrl.btn_radio2.setEnabled(not is_enabled)
        left_panel_ctrl.jog_speed_slider.setEnabled(not is_enabled)

        for i in range(left_panel_ctrl.button_stack.count()):
            frame = self._ui.left_panel.ctrl.button_stack.widget(i)
            buttons = frame.findChildren(QPushButton)
            for button in buttons:
                button.setEnabled(not is_enabled)

        left_panel_ctrl.btn_home_pos.setEnabled(not is_enabled)
        central_btm_panel.btn_teach_pos.setEnabled(not is_enabled)
        self._toggle_predefined_programs(is_enabled)

        self._disable_prog_ctrl()

        # self._ui.setup_save_btn.setEnabled(is_enabled)

    def _toggle_predefined_programs(self, is_enabled: bool):
        for key in [
            "robot_action:ping",
            "robot_action:test_switches",
            "robot_action:home",
        ]:
            action = self._ui.get_action_by_data(key)
            if action:
                action.setEnabled(not is_enabled)
            else:
                print(f"Missing action for: {key}")

    def _disable_prog_ctrl(self):
        right_panel = self._ui.right_panel
        btns = [right_panel.load_btn, right_panel.run_btn, right_panel.stop_btn]
        for btn in btns:
            if btn.isEnabled():
                btn.setEnabled(False)

    def update_connection_status(self, status_text):
        """Update connection status label"""
        con_status_label = self._ui.left_panel.con.con_status_label
        con_status_label.setText(status_text)

    def update_slider_label(self, val: int):
        self._ui.left_panel.ctrl.label_speed = val
        self._ui.btm_bar.speed = val

    def update_com_monitor(
        self,
        dir: str,
        type: str,
        msg: str,
        received_bytes: bytes | None = None,
        parsed: str = "",
    ):
        is_raw_data = bool(received_bytes)
        com_monitor_component = self._ui.com_monitor_panel
        if is_raw_data:
            com_monitor_component.add_log_entry(dir, type, msg, received_bytes, parsed)
        else:
            com_monitor_component.add_log_entry(dir, type, msg)

    def display_joint_angles(self, steps: list[int]):
        joint_angles = self.R.get_joint_angles_deg(steps)
        for i in range(len(joint_angles)):
            joint_angles[i] = round(joint_angles[i], 2)
        self._ui.central_btm_panel.set_joint_positions(joint_angles)

    def display_fk_pose(self, steps: list[int]):
        joint_angles_rad = self.R.steps_to_q_rad(steps)
        T = self.R.fkine(joint_angles_rad)
        x, y, z = T.t * 1000  # in mm
        rx, ry, rz = T.rpy(order="xyz", unit="deg")
        self._ui.central_btm_panel.set_tool_position([x, y, z, rx, ry, rz])

    # def update_program_monitor(
    #     self,
    #     cmd,
    #     font_size=11,
    #     font_weight=600,
    #     txt_color="lightblue",
    #     bracket_color="white",
    #     with_brackets=True,
    # ):
    #     """Update program monitor display"""
    #     self._ui.prog_textEdit.clear()
    #     cursor = self._ui.prog_textEdit.textCursor()
    #     cursor.movePosition(QTextCursor.MoveOperation.End)

    #     # Format command
    #     fmt_cmd = QTextCharFormat()
    #     fmt_cmd.setForeground(QColor(txt_color))
    #     fmt_cmd.setFontWeight(font_weight)
    #     fmt_cmd.setFontPointSize(font_size)

    #     # Format brackets
    #     fmt_brackets = QTextCharFormat()
    #     fmt_brackets.setForeground(QColor(bracket_color))
    #     fmt_brackets.setFontWeight(font_weight)
    #     fmt_brackets.setFontPointSize(font_size)

    #     cursor.insertText(cmd, fmt_cmd)
    #     if with_brackets:
    #         cursor.insertText("()", fmt_brackets)
    #     cursor.insertText("\n")

    #     self._ui.prog_textEdit.setTextCursor(cursor)

    # def log_message(self, log_prefix, data, prefix_color):
    #     """Log a message with formatting"""
    #     date = str(datetime.datetime.now().date())
    #     time = str(datetime.datetime.now().time()).split(".")[0]
    #     dt_time_str = f"{date} {time} - "

    #     cursor = self._ui.log_textEdit.textCursor()
    #     cursor.movePosition(QTextCursor.MoveOperation.End)

    #     fmt_prefix = QTextCharFormat()
    #     fmt_prefix.setForeground(QColor(prefix_color))
    #     fmt_prefix.setFontWeight(400)
    #     fmt_prefix.setFontPointSize(9)

    #     fmt_dt_time = QTextCharFormat()
    #     fmt_dt_time.setForeground(QColor("white"))
    #     fmt_dt_time.setFontWeight(400)
    #     fmt_dt_time.setFontPointSize(9)

    #     cursor.insertText(f"[{log_prefix}]", fmt_prefix)
    #     cursor.insertText(" ")
    #     cursor.insertText(dt_time_str, fmt_dt_time)
    #     cursor.insertText(data + ("\n" if "\n" not in data else ""))

    #     self._ui.log_textEdit.setTextCursor(cursor)

    # def update_log_monitor_with_serial(self, data):
    #     """Update log monitor with serial data"""
    #     self.log_message("LOG", data, "lightgreen")

    # def process_data_update(self, data):
    #     """Process data updates from serial"""
    #     data_content = data[5:].strip()
    #     if "MOTOR_POS_STEPS" in data_content:
    #         self._update_jog_label(data_content)
    #     elif "FK_POSE" in data_content:
    #         self._update_fk_pose_labels(data_content)
    #     elif "JOINT_ANGLES" in data_content:
    #         self._update_joint_angle_labels(data_content)
    #     elif "L_SWITCH" in data_content:
    #         self._update_limit_switch_display(data_content)

    # def _update_jog_label(self, data_content):
    #     """Update jog position labels"""
    #     match = re.search(r"\*(.*)", data_content)
    #     if match:
    #         motor, pos = match.group(1).split(",")
    #         label_name = f"jog_j{motor}_label"
    #         label_widget = getattr(self._ui, label_name, None)
    #         if label_widget is not None:
    #             label_widget.setText(pos)

    # def _update_fk_pose_labels(self, data_content):
    #     """Update forward kinematics pose labels"""
    #     match = re.search(r"\*(.*)", data_content)
    #     if match:
    #         pose_values = match.group(1).split(",")
    #         if len(pose_values) == 6:
    #             x, y, z, roll, pitch, yaw = pose_values
    #             self._ui.tool_x_label.setText(x)
    #             self._ui.tool_y_label.setText(y)
    #             self._ui.tool_z_label.setText(z)
    #             self._ui.tool_rx_label.setText(roll)
    #             self._ui.tool_ry_label.setText(pitch)
    #             self._ui.tool_rz_label.setText(yaw)

    # def _update_joint_angle_labels(self, data_content):
    #     """Update joint angle labels"""
    #     match = re.search(r"\*(.*)", data_content)
    #     if match:
    #         angles_values = match.group(1).split(",")
    #         if len(angles_values) == 6:
    #             for i, angle in enumerate(angles_values, start=1):
    #                 label_name = f"joint_pos_j{i}_label"
    #                 label_widget = getattr(self._ui, label_name, None)
    #                 if label_widget is not None:
    #                     label_widget.setText(angle)

    # def _update_limit_switch_display(self, data_content):
    #     """ "Update limit switch display"""
    #     match = re.search(r"\*(.*)", data_content)
    #     if match:
    #         switch = match.group(1).split(",")
    #         str = "\n".join(
    #             [
    #                 f"Switch {i+1}: {'HIGH' if int(val) else 'LOW'}"
    #                 for i, val in enumerate(switch[:6])
    #             ]
    #         )
    #         self.update_program_monitor(
    #             str,
    #             font_weight=600,
    #             txt_color="lightgrey",
    #             with_brackets=False,
    #         )
