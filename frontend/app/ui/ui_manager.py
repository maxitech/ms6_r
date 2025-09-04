import datetime
import re
from PySide6.QtGui import QTextCharFormat, QColor, QTextCursor


class UIManager:
    def __init__(self, ui):
        self._ui = ui
        self._current_program = None

    def update_combo_box(self, ports):
        """Update port combo box"""
        self._ui.con_device_comboBox.clear()
        self._ui.con_device_comboBox.addItems(ports)

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
        # self._ui.setup_save_btn.setEnabled(is_enabled)

    def update_connection_status(self, status_text):
        """Update connection status label"""
        con_status_label = self._ui.left_panel.con.con_status_label
        con_status_label.setText(status_text)

    def update_program_monitor(
        self,
        cmd,
        font_size=11,
        font_weight=600,
        txt_color="lightblue",
        bracket_color="white",
        with_brackets=True,
    ):
        """Update program monitor display"""
        self._ui.prog_textEdit.clear()
        cursor = self._ui.prog_textEdit.textCursor()
        cursor.movePosition(QTextCursor.MoveOperation.End)

        # Format command
        fmt_cmd = QTextCharFormat()
        fmt_cmd.setForeground(QColor(txt_color))
        fmt_cmd.setFontWeight(font_weight)
        fmt_cmd.setFontPointSize(font_size)

        # Format brackets
        fmt_brackets = QTextCharFormat()
        fmt_brackets.setForeground(QColor(bracket_color))
        fmt_brackets.setFontWeight(font_weight)
        fmt_brackets.setFontPointSize(font_size)

        cursor.insertText(cmd, fmt_cmd)
        if with_brackets:
            cursor.insertText("()", fmt_brackets)
        cursor.insertText("\n")

        self._ui.prog_textEdit.setTextCursor(cursor)

    def log_message(self, log_prefix, data, prefix_color):
        """Log a message with formatting"""
        date = str(datetime.datetime.now().date())
        time = str(datetime.datetime.now().time()).split(".")[0]
        dt_time_str = f"{date} {time} - "

        cursor = self._ui.log_textEdit.textCursor()
        cursor.movePosition(QTextCursor.MoveOperation.End)

        fmt_prefix = QTextCharFormat()
        fmt_prefix.setForeground(QColor(prefix_color))
        fmt_prefix.setFontWeight(400)
        fmt_prefix.setFontPointSize(9)

        fmt_dt_time = QTextCharFormat()
        fmt_dt_time.setForeground(QColor("white"))
        fmt_dt_time.setFontWeight(400)
        fmt_dt_time.setFontPointSize(9)

        cursor.insertText(f"[{log_prefix}]", fmt_prefix)
        cursor.insertText(" ")
        cursor.insertText(dt_time_str, fmt_dt_time)
        cursor.insertText(data + ("\n" if "\n" not in data else ""))

        self._ui.log_textEdit.setTextCursor(cursor)

    def update_log_monitor_with_serial(self, data):
        """Update log monitor with serial data"""
        self.log_message("LOG", data, "lightgreen")

    def process_data_update(self, data):
        """Process data updates from serial"""
        data_content = data[5:].strip()
        if "MOTOR_POS_STEPS" in data_content:
            self._update_jog_label(data_content)
        elif "FK_POSE" in data_content:
            self._update_fk_pose_labels(data_content)
        elif "JOINT_ANGLES" in data_content:
            self._update_joint_angle_labels(data_content)
        elif "L_SWITCH" in data_content:
            self._update_limit_switch_display(data_content)

    def _update_jog_label(self, data_content):
        """Update jog position labels"""
        match = re.search(r"\*(.*)", data_content)
        if match:
            motor, pos = match.group(1).split(",")
            label_name = f"jog_j{motor}_label"
            label_widget = getattr(self._ui, label_name, None)
            if label_widget is not None:
                label_widget.setText(pos)

    def _update_fk_pose_labels(self, data_content):
        """Update forward kinematics pose labels"""
        match = re.search(r"\*(.*)", data_content)
        if match:
            pose_values = match.group(1).split(",")
            if len(pose_values) == 6:
                x, y, z, roll, pitch, yaw = pose_values
                self._ui.tool_x_label.setText(x)
                self._ui.tool_y_label.setText(y)
                self._ui.tool_z_label.setText(z)
                self._ui.tool_rx_label.setText(roll)
                self._ui.tool_ry_label.setText(pitch)
                self._ui.tool_rz_label.setText(yaw)

    def _update_joint_angle_labels(self, data_content):
        """Update joint angle labels"""
        match = re.search(r"\*(.*)", data_content)
        if match:
            angles_values = match.group(1).split(",")
            if len(angles_values) == 6:
                for i, angle in enumerate(angles_values, start=1):
                    label_name = f"joint_pos_j{i}_label"
                    label_widget = getattr(self._ui, label_name, None)
                    if label_widget is not None:
                        label_widget.setText(angle)

    def _update_limit_switch_display(self, data_content):
        """ "Update limit switch display"""
        match = re.search(r"\*(.*)", data_content)
        if match:
            switch = match.group(1).split(",")
            str = "\n".join(
                [
                    f"Switch {i+1}: {'HIGH' if int(val) else 'LOW'}"
                    for i, val in enumerate(switch[:6])
                ]
            )
            self.update_program_monitor(
                str,
                font_weight=600,
                txt_color="lightgrey",
                with_brackets=False,
            )

    def set_current_program(self, program):
        """Set current program"""
        self._current_program = program

    def get_current_program(self):
        """Get current program"""
        return self._current_program
