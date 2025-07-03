import serial.tools.list_ports
import serial
from app.core.serial_worker import SerialWorker


class SerialConnection:
    def __init__(self, helper, ui_manager, baudrate=115200):
        self._helper = helper
        self._ui_manager = ui_manager
        self._connection_handler = None
        self._serial_worker = None
        self._baudrate = baudrate
        self._serial = None
        self._ports = []
        self._port = None

    def set_connection_handler(self, handler):
        self._connection_handler = handler

    def getPorts(self):
        self._load_ports()
        devices = [port.device for port in self._ports]
        return devices

    def setPort(self, port):
        self._port = port

    def connect(self, update_ui_callback):
        try:
            self._serial = serial.Serial(
                self._port, self._baudrate, timeout=1, write_timeout=1
            )
            self._serial_worker = SerialWorker(self._serial)
            self._serial_worker.data_received.connect(update_ui_callback)
            self._serial_worker.start()
            print(f"Connected to {self._port} at {self._baudrate} baud.")
        except serial.SerialException as e:
            print(f"Error connecting to {self._port}: {e}")

    def disconnect(self):
        if self._serial_worker:
            self._serial_worker.stop()
        if self._serial and self._serial.is_open:
            self._serial.close()
            print(f"Disconnected from {self._port}.")

    def is_connected(self):
        if self._serial and self._serial.is_open:
            available_ports = [
                port.device for port in serial.tools.list_ports.comports()
            ]
            if self._port in available_ports:
                return True
            else:
                print(
                    f"Warning: {self._port} no longer available, device may be disconnected."
                )
                self._connection_handler.handle_unexpected_disconnect()
                self.disconnect()
                return False
        return False

    def _send_data(self, data):
        """Internal send function"""
        if not self.is_connected():
            print(f"Cannot send data, no active connection to {self._port}")
            return

        if self.is_connected():
            if isinstance(data, str):
                data = bytes(data, "utf-8")
            try:
                self._serial.write(data)
            except serial.SerialTimeoutException:
                print("Write timeout error!")
            except Exception as e:
                print(f"[MainWindow] Write error: {e}")

    def send_data(self, data):
        """Public Send data function."""
        if isinstance(data, str) and len(data) > 0:
            checksum = self._helper.calc_checksum(data)
            data_str = f"${data}*{checksum}#"

            if self.is_connected():
                if self._is_valid_format(data_str):
                    self._send_data(data_str)
                    print(f"Sent: {data_str}")
                else:
                    self._ui_manager.log_message("ERROR", "Invalid data format!", "red")
            else:
                self._ui_manager.log_message("ERROR", "Not connected!", "red")

    def _is_valid_format(self, data_str):
        """Check if data format is valid"""
        return (
            data_str.startswith("$")
            and data_str.find("*") != -1
            and data_str.endswith("#")
        )

    # private methods
    def _load_ports(self):
        self._ports = serial.tools.list_ports.comports()
