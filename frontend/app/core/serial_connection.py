import serial.tools.list_ports
from serial import Serial
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from app.handlers.program_handler import ProgramHandler
    from app.handlers.connection_handler import ConnectionHandler
from app.core.threads.serial_reader_thread import SerialReaderThread
from app.core.threads.serial_writer_thread import SerialWriterThread
from app.core.threads.motion_planner_thread import MotionPlannerThread
from app.constants.com_protocol import (
    START_BYTES,
    END_BYTES,
    CMD_START,
    CMD_STOP,
    CMD_IDLE,
    CMD_SETUP,
    CMD_JOG,
    CMD_MOVE_TO_POS,
    CMD_LOAD,
    PRG_PING,
    PRG_TEST_SWITCHES,
    PRG_HOME,
    PRG_MAIN,
)
from app.core.shared.shared_data import shared_data
from app.ui.ui_manager import UIManager


class SerialConnection(Serial):
    def __init__(self, helper, ui_manager: UIManager, baudrate=115200):
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

    def set_connection_handler(self, handler: "ConnectionHandler"):
        self._connection_handler = handler

    def set_prog_handler(self, prog_handler: "ProgramHandler"):
        if self._connection_handler:
            self._connection_handler.set_prog_handler(prog_handler)

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
        except serial.SerialException as e:
            print(f"Error connecting to {self._port}: {e}")
            self._ui_manager.update_com_monitor(
                "sys", "error", f"Error connecting to {self._port}: {e}"
            )

    def disconnect(self):
        if self._serial_reader_t and self._serial_writer_t and self._motion_planner_t:
            self._serial_reader_t.stop()
            self._serial_writer_t.stop()
            self._motion_planner_t.stop()
            shared_data.unsubscribe()
        if self._serial and self._serial.is_open:
            self._serial.close()
            self._ui_manager.update_com_monitor(
                "sys", "info", f"Disconnected from {self._port}."
            )

    def is_connected(self):
        if self._serial and self._serial.is_open:
            available_ports = [
                port.device for port in serial.tools.list_ports.comports()
            ]
            if self._port in available_ports:
                return True
            else:
                if self._connection_handler:
                    self._connection_handler.handle_unexpected_disconnect()
                self.disconnect()
                return False
        return False

    def _set_data_out(self, data):
        if isinstance(data, bytes):
            shared_data.set_data_out(data)

    def set_data_out(self, data: bytes, data_str: str = ""):
        """Public Send data function."""
        if isinstance(data, bytes) and len(data) > 0:
            if self.is_connected():
                if self._is_valid_packet(data):
                    self._set_data_out(data)
                    if data[0] == ord("$") and data[-1] == ord("#"):
                        self._ui_manager.update_com_monitor(
                            "tx", "data", "Sending setup to Robot."
                        )
                    else:
                        self._log_tx(data, data_str)
                else:
                    self._ui_manager.update_com_monitor(
                        "sys", "error", "ERROR: Invalid data format!"
                    )
            else:
                self._ui_manager.update_com_monitor(
                    "sys", "error", "ERROR Not connected!"
                )

    def _is_valid_packet(self, packet: bytes):
        """Check if data format is valid"""
        if packet[0:3] == bytes(START_BYTES) and packet[-2:] == bytes(END_BYTES):
            return True
        elif packet[0] == ord("$") and packet[-1] == ord("#"):
            return True

    def _load_ports(self):
        self._ports = serial.tools.list_ports.comports()

    def _log_tx(self, data: bytes, data_str: str):
        CMD_MAP = {
            CMD_START: "Start",
            CMD_STOP: "Stop",
            CMD_IDLE: "Idle",
            CMD_SETUP: "Setup",
            CMD_JOG: "Jog",
            CMD_MOVE_TO_POS: "Move To Position",
            CMD_LOAD: "Load",
        }
        PRG_MAP = {
            PRG_PING: "Ping",
            PRG_TEST_SWITCHES: "Test Switches",
            PRG_HOME: "Home Robot",
            PRG_MAIN: "Main",
        }

        cmd = data[4]
        prg = data[5] if cmd == CMD_LOAD else None

        cmd_name = CMD_MAP.get(cmd, "Unknown Command")
        tx_msg = f"TX: {cmd_name.upper()}"

        if cmd == CMD_LOAD and prg is not None:
            prg_name = PRG_MAP.get(prg, "Unknown Program")
            if not data_str:
                parsed_cmd_label = f"Program={prg_name.upper()}"
            else:
                parsed_cmd_label = f"Program={prg_name.upper()}::{data_str}"
        else:
            if not data_str:
                parsed_cmd_label = f"Execute={cmd_name.upper()}"
            else:
                parsed_cmd_label = f"Execute={cmd_name.upper()}::{data_str}"

        self._ui_manager.update_com_monitor(
            "tx", "data", tx_msg, data, parsed_cmd_label
        )


# cmd = data[4]
# # if cmd is load then the byte after cmd is the program
# prg = data[5]

# tx_msg = f"Transmitting '{CMD_MAP.get(cmd, "Unknown Command").upper()}' command to robot."

# parsed_cmd_label = ""
# if cmd == CMD_LOAD:
#     parsed_cmd_label = f"Loading program: {PRG_MAP.get(prg, "Unknown Program").upper()} to robot."
# else:
#     parsed_cmd_label = (
#         f"Executing: {CMD_MAP.get(cmd, "Unknown Command").upper()}"
#     )
# self._ui_manager.update_com_monitor(
#     "tx", "data", f"{tx_msg}", data, parsed_cmd_label
# )
