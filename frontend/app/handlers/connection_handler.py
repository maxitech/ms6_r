import json
from app.core.serial_connection import SerialConnection
from typing import List
from app.core.shared.shared_data import shared_data
from app.utils.helper import Helper
from app.constants.com_protocol import *
from app.core.packet_builder import PacketBuilder
from app.core.packet_processor import PacketProcessor
from app.core.packet_parser import PacketParser

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from main import MainWindow


class ConnectionHandler:
    def __init__(self, ui: "MainWindow", setup, serial: SerialConnection, ui_manager):
        self._ui = ui
        self._setup = setup
        self._serial = serial
        self._ui_manager = ui_manager
        self._helper = Helper()
        self._pb = PacketBuilder()
        self._pp = PacketProcessor()
        self._parser = PacketParser()
        self._current_ports: List[str] = []
        shared_data.subscribe("new_steps", self._update_ui)

    def _update_ui(self, data):
        print("Updated UI", data)

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
        self._serial.connect(self._on_serial_data_received)  # provide callback

        if self._serial.is_connected():
            data = f"SETUP,[{json.dumps(self._setup.get_setup())}]"  # create packet
            checksum = self._helper.calc_checksum(data)
            setup_data_str = f"${data}*{checksum}#"

            if self._is_valid_format(setup_data_str):
                setup_bytes = setup_data_str.encode("utf-8")
                self._serial.set_data_out(setup_bytes)

            self._ui_manager.update_ui_based_on_connection_status(
                "Disconnect", f"Connected to {selected_port}", False
            )
            self._ui_manager.log_message("INFO", "Connection established", "lightblue")
        else:
            self._ui_manager.update_connection_status(
                f"Failed to connect to {selected_port}"
            )
            self._ui_manager.log_message("ERROR", "Failed to connect", "red")

    def _is_valid_format(self, data_str):
        """Check if data format is valid"""
        return (
            data_str.startswith("$")
            and data_str.find("*") != -1
            and data_str.endswith("#")
        )

    def _disconnect(self):
        """Disconnect from current port"""
        packet: bytes = self._pb.build_packet(cmd_id=CMD_IDLE, data=NOP)
        self._serial.set_data_out(packet)
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

    def _on_serial_data_received(self, raw_data: bytes):
        packets: list[bytes] = self._parser.feed(raw_data)
        for p in packets:
            self._pp.process_packet(p)

        # Trigger UI update
        # shared_data

        # Trigger new calculations

        # print("dta: ", raw_data)

        # only updates if prev != new dta
        # pass to shared data or call shared dta fn's
        # steps = extract_steps_from_data(raw_data)
        # shared_data.update_steps(steps)
