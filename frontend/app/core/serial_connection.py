import serial.tools.list_ports
from serial import Serial
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from app.handlers.connection_handler import ConnectionHandler
from app.core.threads.serial_reader_thread import SerialReaderThread
from app.core.threads.serial_writer_thread import SerialWriterThread
from app.core.threads.motion_planner_thread import MotionPlannerThread
from app.constants.com_protocoll import START_BYTES, END_BYTES
from app.core.shared.shared_data import shared_data


class SerialConnection(Serial):
    def __init__(self, helper, ui_manager, baudrate=115200):
        self._helper = helper
        self._ui_manager = ui_manager
        self._connection_handler: ConnectionHandler | None = None
        self._serial_reader_t = None
        self._serial_writer_t = None
        self._motion_planner_t = None
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

    def connect(self, on_serial_data_received):
        try:
            self._serial = serial.Serial(
                self._port, self._baudrate, timeout=1, write_timeout=1
            )
            self._serial_reader_t = SerialReaderThread(self._serial)
            self._serial_reader_t.data_received.connect(on_serial_data_received)
            self._serial_reader_t.start()

            self._serial_writer_t = SerialWriterThread(self._serial)
            self._serial_writer_t.start()

            self._motion_planner_t = MotionPlannerThread(self._serial)
            self._motion_planner_t.start()
            print(f"Connected to {self._port} at {self._baudrate} baud.")
        except serial.SerialException as e:
            print(f"Error connecting to {self._port}: {e}")

    def disconnect(self):
        if self._serial_reader_t and self._serial_writer_t and self._motion_planner_t:
            self._serial_reader_t.stop()
            self._serial_writer_t.stop()
            self._motion_planner_t.stop()
            shared_data.unsubscribe()
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
                if self._connection_handler:
                    self._connection_handler.handle_unexpected_disconnect()
                self.disconnect()
                return False
        return False

    def _set_data_out(self, data):
        if isinstance(data, bytes):
            shared_data.set_data_out(data)

    def set_data_out(self, data):
        """Public Send data function."""
        if isinstance(data, bytes) and len(data) > 0:
            if self.is_connected():
                if self._is_valid_packet(data):
                    self._set_data_out(data)
                else:
                    self._ui_manager.log_message("ERROR", "Invalid data format!", "red")
            else:
                self._ui_manager.log_message("ERROR", "Not connected!", "red")

    def _is_valid_packet(self, packet: bytes):
        """Check if data format is valid"""
        return packet[0:3] == bytes(START_BYTES) and packet[-2:] == bytes(END_BYTES)

    def _load_ports(self):
        self._ports = serial.tools.list_ports.comports()
