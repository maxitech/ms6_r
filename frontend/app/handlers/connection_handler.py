import json


class ConnectionHandler:
    def __init__(self, ui, setup, serial, ui_manager):
        self._ui = ui
        self._setup = setup
        self._serial = serial
        self._ui_manager = ui_manager
        self._current_ports = []

    def setup_connections(self):
        """Setup connection-related UI connections"""
        self._ui.con_connect_btn.clicked.connect(self._handle_con_btn_click)
        self.check_ports()

    def check_ports(self):
        """Check available ports and update UI"""
        ports = self._serial.getPorts()
        if ports != self._current_ports:
            self._current_ports = ports
            self._ui_manager.update_combo_box(ports)

    def check_status(self):
        """Check connection status"""
        if not self._serial.is_connected():
            self._ui_manager.update_ui_based_on_connection_status(
                "Connect", "Disconnected", True
            )

    def _handle_con_btn_click(self):
        """Handle connect/disconnect button click"""
        if self._ui.con_connect_btn.text() == "Connect":
            self._connect()
        else:
            self._disconnect()

    def _connect(self):
        """Connect to selected port"""
        selected_port = self._ui.con_device_comboBox.currentText()
        self._serial.setPort(selected_port)
        self._serial.connect(self._process_received_data)

        if self._serial.is_connected():
            setup_data = f"SETUP,[{json.dumps(self._setup.get_setup())}]"
            self._serial.send_data(setup_data)
            self._ui_manager.update_ui_based_on_connection_status(
                "Disconnect", f"Connected to {selected_port}", False
            )
            self._ui_manager.log_message("INFO", "Connection established", "lightblue")
        else:
            self._ui_manager.update_connection_status(
                f"Failed to connect to {selected_port}"
            )
            self._ui_manager.log_message("ERROR", "Failed to connect", "red")

    def _disconnect(self):
        """Disconnect from current port"""
        self._serial.send_data("IDLE,[EXEC]")
        self._serial.disconnect()
        if not self._serial.is_connected():
            self._ui_manager.update_ui_based_on_connection_status(
                "Connect", "Disconnected", True
            )
            self._ui_manager.log_message("INFO", "Disconnected", "lightblue")
        else:
            self._ui_manager.update_connection_status("Failed to disconnect")
            self._ui_manager.log_message("ERROR", "Failed to disconnect", "red")

    def handle_unexpected_disconnect(self):
        """Handle unexpected disconnection"""
        self._ui_manager.log_message(
            "ERROR", "Connection lost! Device may be disconnected.", "red"
        )

    def _process_received_data(self, data):
        """Process received serial data"""
        data = data.strip()
        if not data.startswith("DATA:"):
            self._ui_manager.update_log_monitor_with_serial(data)
        else:
            self._ui_manager.process_data_update(data)
