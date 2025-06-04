import datetime
import re
from PySide6.QtGui import QTextCharFormat, QColor, QTextCursor
from PySide6.QtCore import QTimer
from app.core.serial_connection import SerialConnection
from app.utils.helper import Helper


class MainWindowController:
    def __init__(self, ui):
        self._ui = ui
        self._serial = SerialConnection(controller=self)
        self._helper = Helper()
        self._current_ports = []

        self._check_ports()
        self._port_timer = QTimer()
        self._port_timer.timeout.connect(self._check_ports)
        self._port_timer.timeout.connect(self._check_status)
        self._port_timer.start(2000)

        self._program = None

        self._jog_joint = ""
        self._jog_direction = ""

        # Connect btn
        self._ui.con_connect_btn.clicked.connect(self._handle_con_btn_click)

        # Cmd btn's
        self._ui.cmd_ping_btn.clicked.connect(
            lambda: self._handle_cmd_btn_click(self._ui.cmd_ping_btn)
        )
        self._ui.cmd_pong_btn.clicked.connect(
            lambda: self._handle_cmd_btn_click(self._ui.cmd_pong_btn)
        )
        self._ui.cmd_switches_btn.clicked.connect(
            lambda: self._handle_cmd_btn_click(self._ui.cmd_switches_btn)
        )
        self._ui.cmd_home_axis_btn.clicked.connect(
            lambda: self._handle_cmd_btn_click(self._ui.cmd_home_axis_btn)
        )

        # Button container
        self._ui.btn_start.clicked.connect(self._handle_start_btn_click)
        self._ui.btn_stop.clicked.connect(self._handle_stop_btn_click)
        self._ui.btn_load_prog_btn.clicked.connect(self._handle_load_btn_click)

        # Prog Clear btn
        self._ui.prog_clear_btn.clicked.connect(self._handle_clear_btn_click)

        # Jog btn's
        jog_buttons = [
            (self._ui.jog_j1_pos_btn),
            (self._ui.jog_j1_neg_btn),
            (self._ui.jog_j2_pos_btn),
            (self._ui.jog_j2_neg_btn),
            (self._ui.jog_j3_pos_btn),
            (self._ui.jog_j3_neg_btn),
            (self._ui.jog_j4_pos_btn),
            (self._ui.jog_j4_neg_btn),
            (self._ui.jog_j5_pos_btn),
            (self._ui.jog_j5_neg_btn),
            (self._ui.jog_j6_pos_btn),
            (self._ui.jog_j6_neg_btn),
        ]
        for button in jog_buttons:
            button.pressed.connect(lambda btn=button: self._handle_jog_btn_press(btn))
            button.released.connect(lambda: self._handle_jog_btn_release())

        # Jog Slider
        self._ui.jog_slider_label.setText(
            str(self._ui.jog_slider.value())
        )  # Initial value
        self._ui.jog_slider.valueChanged.connect(self._handle_jog_slider_change)
        self._ui.jog_slider.sliderReleased.connect(self._handle_jog_slider_value)

    # *************************Public Methods****************************
    def handle_unexpected_disconnect(self):
        self._fmt_log_monitor(
            "[ERROR]", "Connection lost! Device may be disconnected.", "red"
        )

    # *************************Private Methods****************************
    def _check_ports(self):
        _ports = self._serial.getPorts()
        if _ports != self._current_ports:
            self._current_ports = _ports
            self._update_combo_box()

    def _connect(self):
        selected_port = self._ui.con_device_comboBox.currentText()
        self._serial.setPort(selected_port)
        self._serial.connect(self._process_received_data)
        if self._serial.is_connected() and not None:
            self._update_ui_based_on_connection_status(
                "Disconnect", f"Connected to {selected_port}", False
            )
            self._fmt_log_monitor("[INFO]", "Connection established", "lightblue")
        else:
            self._ui.con_status_label2.setText(f"Failed to connect to {selected_port}")
            self._fmt_log_monitor("[ERROR]", "Failed to connect", "red")

    def _disconnect(self):
        self._serial.disconnect()
        if not self._serial.is_connected():
            self._update_ui_based_on_connection_status("Connect", "Disconnected", True)
            self._fmt_log_monitor("[INFO]", "Disconnected", "lightblue")
        else:
            self._ui.con_status_label2.setText("Failed to disconnect")
            self._fmt_log_monitor("[ERROR]", "Failed to disconnect", "red")

    def _check_status(self):
        if not self._serial.is_connected():
            self._update_ui_based_on_connection_status("Connect", "Disconnected", True)
        else:
            return

    def _send_data(self, data):
        text_input = data
        if isinstance(text_input, str) and len(text_input) > 0:
            checksum = self._helper.calc_checksum(text_input)
            data_str = f"${text_input}*{checksum}#"

            if self._serial.is_connected():
                if (
                    data_str.startswith("$")
                    and data_str.find("*") != -1
                    and data_str.endswith("#")
                ):
                    self._serial.send_data(data_str)
                    print(f"Sent: {data_str}")
                else:
                    self._fmt_log_monitor("[ERROR]", "Invalid data format!", "red")
            else:
                self._fmt_log_monitor("[ERROR]", "Not connected!", "red")
                return

    def _process_received_data(self, data):
        # ? make ui updates which in relation to received serial data here
        # print(f"Received in UI: {data}")
        data = data.strip()
        if not data.startswith("DATA:"):
            self._update_log_monitor_with_serial(data)
        else:
            # Handle data that starts with "DATA:"
            data_content = data[5:].strip()
            if "MOTOR_POS_STEPS" in data_content:
                match = re.search(r"\*(.*?)#(.*)", data_content)
                if match:
                    motor = match.group(1)
                    pos = match.group(2)
                    label_name = f"jog_j{motor}_label"
                    label_widget = getattr(self._ui, label_name, None)
                    if label_widget is not None:
                        label_widget.setText(pos)

    # ***************Handlers*******************
    def _handle_con_btn_click(self):
        if self._ui.con_connect_btn.text() == "Connect":
            self._connect()
        else:
            self._disconnect()

    def _handle_cmd_btn_click(self, clicked_button):
        cmd_map = {
            self._ui.cmd_ping_btn: ("Ping", "LOAD,[PING]"),
            self._ui.cmd_pong_btn: ("Pong", "LOAD,[PONG]"),
            self._ui.cmd_switches_btn: ("TestSwitches", "LOAD,[TEST_SWITCHES]"),
            self._ui.cmd_home_axis_btn: ("Home", "LOAD,[HOME]"),
        }

        if clicked_button in cmd_map:
            name, command = cmd_map[clicked_button]
            if self._program == command:
                return
            self._fmt_program_monitor(name, "lightblue", "white")
            self._program = command
            self._ui.btn_load_prog_btn.setEnabled(True)

    def _handle_load_btn_click(self):
        if self._check_for_program():
            self._send_data(self._program)
            self._ui.btn_start.setEnabled(True)
            self._ui.btn_stop.setEnabled(False)

    def _handle_start_btn_click(self):
        if self._check_for_program():
            self._send_data("START,[EXEC]")
            self._ui.btn_start.setEnabled(False)
            self._ui.btn_stop.setEnabled(True)

    def _handle_stop_btn_click(self):
        if self._check_for_program():
            self._send_data("STOP,[EXEC]")
            self._ui.btn_stop.setEnabled(False)
            self._ui.btn_start.setEnabled(True)

    def _handle_clear_btn_click(self):
        self._fmt_log_monitor("[INFO]", "Program cleared", "lightblue")
        self._send_data("IDLE,[EXEC]")
        self._program = None
        self._ui.btn_load_prog_btn.setEnabled(False)
        self._ui.btn_start.setEnabled(False)
        self._ui.btn_stop.setEnabled(False)

    def _handle_jog_btn_press(self, btn):
        btn_name = btn.objectName()
        parts = btn_name.split("_")
        if len(parts) == 4:
            self._jog_joint = parts[1].upper()
            self._jog_direction = parts[2].upper()
            start_data = f"JOG,[{self._jog_joint}, {self._jog_direction}, START]"
            self._send_data(start_data)
            print(start_data)
        else:
            print(f"Error: Object-Name '{btn_name}' has an unexpected structure!")

    def _handle_jog_btn_release(self):
        stop_data = f"JOG,[{self._jog_joint}, {self._jog_direction}, STOP]"
        self._send_data(stop_data)
        print(stop_data)

    # ? This is only a prototype function, not complete yet
    def _handle_home_axis_btn_click(self):
        if self._serial.is_connected():
            home_data = "LOAD,[HOME]"
            self._send_data(home_data)
            self._fmt_program_monitor("Home", "lightblue", "white")
        else:
            self._fmt_log_monitor("[ERROR]", "Cannot home axes, not connected!", "red")

    def _handle_jog_slider_change(self):
        self._ui.jog_slider.setValue(round(self._ui.jog_slider.value() / 10) * 10)
        slider_val = self._ui.jog_slider.value()
        self._ui.jog_slider_label.setText(str(slider_val))

    def _handle_jog_slider_value(self):
        print(self._ui.jog_slider.value())  # Temp for debugging

    # *****************Update UI*********************
    def _update_combo_box(self):
        self._ui.con_device_comboBox.clear()
        self._ui.con_device_comboBox.addItems(self._current_ports)

    def _update_ui_based_on_connection_status(self, btn_text, status_text, is_enabled):
        self._ui.con_connect_btn.setText(btn_text)
        self._ui.con_device_comboBox.setEnabled(is_enabled)
        self._ui.con_status_label2.setText(status_text)

    # *Program Monitor
    def _fmt_program_monitor(self, cmd, txt_color, bracket_color):
        self._ui.prog_textEdit.clear()
        cursor = self._ui.prog_textEdit.textCursor()
        cursor.movePosition(QTextCursor.End)

        # Format <cmd>
        fmt_cmd = QTextCharFormat()
        fmt_cmd.setForeground(QColor(txt_color))
        fmt_cmd.setFontWeight(600)
        fmt_cmd.setFontPointSize(11)

        # Format for "()"
        fmt_brackets = QTextCharFormat()
        fmt_brackets.setForeground(QColor(bracket_color))
        fmt_brackets.setFontWeight(600)
        fmt_brackets.setFontPointSize(11)

        # Insert text
        cursor.insertText(cmd, fmt_cmd)
        cursor.insertText("()", fmt_brackets)
        cursor.insertText("\n")

        self._ui.prog_textEdit.setTextCursor(cursor)

    # *Log Monitor
    def _update_log_monitor_with_serial(self, data):
        self._fmt_log_monitor("[LOG]", data, "lightgreen")

    def _fmt_log_monitor(self, log_prefix, data, prefix_color):
        date = str(datetime.datetime.now().date())
        time = str(datetime.datetime.now().time()).split(".")[0]
        dt_time_str = f"{date} {time} - "

        cursor = self._ui.log_textEdit.textCursor()
        cursor.movePosition(QTextCursor.End)

        fmt_prefix = QTextCharFormat()
        fmt_prefix.setForeground(QColor(prefix_color))
        fmt_prefix.setFontWeight(400)
        fmt_prefix.setFontPointSize(9)

        fmt_dt_time = QTextCharFormat()
        fmt_dt_time.setForeground(QColor("white"))
        fmt_dt_time.setFontWeight(400)
        fmt_dt_time.setFontPointSize(9)

        # Insert text
        cursor.insertText(log_prefix, fmt_prefix)
        cursor.insertText(" ")
        cursor.insertText(dt_time_str, fmt_dt_time)
        cursor.insertText(data + ("\n" if "\n" not in data else ""))

        self._ui.log_textEdit.setTextCursor(cursor)

    # ***********Helper Functions****************
    def _check_for_program(self):
        if self._program is not None:
            return True
        else:
            self._fmt_log_monitor("[ERROR]", "No program loaded!", "red")
            return False
