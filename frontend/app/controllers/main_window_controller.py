from PySide6.QtCore import QTimer
from app.core.serial_connection import SerialConnection
from app.utils.helper import Helper


class MainWindowController:
    def __init__(self, ui):
        self._ui = ui
        self._serial = SerialConnection()
        self._helper = Helper()
        self._current_ports = []

        self._check_ports()
        self._port_timer = QTimer()
        self._port_timer.timeout.connect(self._check_ports)
        self._port_timer.timeout.connect(self._check_status)
        self._port_timer.start(2000)

        self._ui.con_connect_btn.clicked.connect(self._handle_con_btn_click)

    # private methods
    def _check_ports(self):
        _ports = self._serial.getPorts()
        if _ports != self._current_ports:
            self._current_ports = _ports
            self._update_combo_box()

    def _update_combo_box(self):
        self._ui.con_device_comboBox.clear()
        self._ui.con_device_comboBox.addItems(self._current_ports)

    def _handle_con_btn_click(self):
        if self._ui.con_connect_btn.text() == "Connect":
            self._connect()
        else:
            self._disconnect()

    def _connect(self):
        selected_port = self._ui.con_device_comboBox.currentText()
        self._serial.setPort(selected_port)
        self._serial.connect()
        if self._serial.is_connected() and not None:
            self._update_ui_based_on_connection_status(
                "Disconnect", f"Connected to {selected_port}", False
            )
        else:
            self._ui.con_status_label2.setText(f"Failed to connect to {selected_port}")

    def _disconnect(self):
        self._serial.disconnect()
        if not self._serial.is_connected():
            self._update_ui_based_on_connection_status("Connect", "Disconnected", True)
        else:
            self._ui.con_status_label2.setText("Failed to disconnect")

    def _check_status(self):
        if not self._serial.is_connected():
            self._update_ui_based_on_connection_status("Connect", "Disconnected", True)
        else:
            return

    def _update_ui_based_on_connection_status(self, btn_text, status_text, is_enabled):
        self._ui.con_connect_btn.setText(btn_text)
        self._ui.con_device_comboBox.setEnabled(is_enabled)
        self._ui.con_status_label2.setText(status_text)

    def _send_data(self):
        text_input = self._ui.prog_textEdit.toPlainText()
        if isinstance(text_input, str and text_input > 0):
            checksum = self._helper.calc_checksum(text_input)
            data_str = f"${text_input}*{checksum}#"

        if (
            data_str.startswith("#")
            and data_str.find("*") != -1
            and data_str.endswith("#")
        ):
            self._serial.send_data(data_str)
