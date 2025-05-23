import datetime
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

        # Button container
        self._ui.btn_load_prog_btn.clicked.connect(self._handle_load_btn_click)

        # Prog Clear btn
        self._ui.prog_clear_btn.clicked.connect(self._handle_clear_btn_click)

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
        # ! change later
        # text_input = self._ui.prog_textEdit.toPlainText()
        text_input = data
        if isinstance(text_input, str) and len(text_input) > 0:
            checksum = self._helper.calc_checksum(text_input)
            data_str = f"${text_input}*{checksum}#"

            if (
                data_str.startswith("$")
                and data_str.find("*") != -1
                and data_str.endswith("#")
            ):
                self._serial.send_data(data_str)

    def _process_received_data(self, data):
        # ? make ui updates which in relation to received serial data here
        # print(f"Received in UI: {data}")
        self._update_log_monitor_with_serial(data)

    # ***************Handlers*******************
    def _handle_con_btn_click(self):
        if self._ui.con_connect_btn.text() == "Connect":
            self._connect()
        else:
            self._disconnect()

    def _handle_cmd_btn_click(self, clicked_button):
        cmd_map = {
            self._ui.cmd_ping_btn: ("Ping", "LOAD,PING"),
            self._ui.cmd_pong_btn: ("Pong", "LOAD,PONG"),
            self._ui.cmd_switches_btn: ("TestSwitches", "LOAD,TEST_SWITCHES"),
        }

        if clicked_button in cmd_map:
            name, command = cmd_map[clicked_button]
            if self._program == command:
                return
            self._fmt_program_monitor(name, "lightblue", "white")
            self._program = command
            self._ui.btn_load_prog_btn.setEnabled(True)

    def _handle_load_btn_click(self):
        if self._program is not None:
            self._send_data(self._program)

    def _handle_clear_btn_click(self):
        self._program = None
        self._ui.btn_load_prog_btn.setEnabled(False)

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
        self._fmt_log_monitor("[LOG]", data, "red")

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
