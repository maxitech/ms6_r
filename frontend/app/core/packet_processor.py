import crcmod
from app.constants.com_protocol import *
from app.core.shared.shared_data import shared_data
from app.ui.ui_manager import UIManager


class PacketProcessor:
    def __init__(self, ui_manager: UIManager):
        self._ui_manager = ui_manager

        self._cmd_id = None
        self._status = None
        self._detail = None

    def process_packet(self, raw_data: bytes):
        if not self._validate_packet(raw_data):
            print("Invalid packet received.")
            return

        self._raw_data = raw_data
        # extract data
        pl_len = raw_data[3]
        payload = raw_data[4 : 4 + pl_len]

        self._cmd_id = payload[0]
        self._status = payload[1]
        self._detail = payload[2]

        match self._status:
            case _ if self._status == STATUS_OK:
                match self._detail:
                    case _ if self._detail == DATA_NONE:
                        self._log_rx("info", "[INFO] No data available.")
                        # print("[INFO] No data available.")

                    case _ if self._detail == DATA_STEPS:
                        steps: list[int] = self._extract_steps(payload[3:])
                        shared_data.update_steps(steps)

                    case _ if self._detail == INFO_LOADED_PROGRAM:
                        self._log_rx(
                            "info", f"[INFO] Program: 0x{payload[3]:02X} loaded."
                        )

                    case _ if self._detail == INFO_RELOADED_PROGRAM:
                        self._log_rx(
                            "info", f"[INFO] Program: 0x{payload[3]:02X} reloaded."
                        )

                    case _ if self._detail == INFO_ALREADY_RUNNING:
                        self._log_rx(
                            "info",
                            f"[INFO] Program: 0x{payload[3]:02X} already running.",
                        )

                    case _ if self._detail == INFO_HOMING_ALREADY_DONE:
                        self._log_rx("info", "[INFO] Homing already done.")

                    case _ if self._detail == INFO_PING:
                        self._log_rx("info", "[INFO] Ping OK.")

                    case _ if self._detail == INFO_REACHED:
                        shared_data.update_data_in(self._detail)
                        self._log_rx("info", "[INFO] Position Reached.")

                    case _ if self._detail == INFO_TRAJ_START_ACK:
                        shared_data.update_data_in(self._detail)
                        self._log_rx("info", "[INFO] Trajectory start acknowledged.")

                    case _ if self._detail == INFO_TRAJ_CHUNK_ACK:
                        # Optional: Chunk-ID aus payload extrahieren wenn nötig
                        chunk_id = payload[3] if len(payload) > 3 else 0
                        shared_data.update_data_in(self._detail)
                        self._log_rx(
                            "info", f"[INFO] Trajectory chunk {chunk_id} acknowledged."
                        )

                    case _ if self._detail == INFO_TRAJ_UPLOAD_COMPLETE:
                        shared_data.update_data_in(self._detail)
                        self._log_rx("info", "[INFO] Trajectory upload complete.")

                    case _ if self._detail == INFO_TRAJ_EXEC_ACK:
                        shared_data.update_data_in(self._detail)
                        self._log_rx("info", "[INFO] Trajectory execution started.")

                    case _ if self._detail == WARN_NOP_IGNORED:
                        self._log_rx("warning", "[WARN] No operation ignored.")

                    case _ if self._detail == WARN_NO_PROGRAM_LOADED:
                        self._log_rx("warning", "[WARN] No program loaded.")

                    case _ if self._detail == WARN_LIMIT_HIT:
                        self._log_rx("warning", "[WARN] Limit reached.")

                    case _ if self._detail == WARN_ARM_NOT_HOMED:
                        self._log_rx("warning", "[WARN] Arm not homed.")

                    case _:
                        print("[ERROR] Unknown STATUS_OK detail")
                        self._log_rx("error", "[ERROR] Unknown STATUS_OK detail")

            case _ if self._status == STATUS_ERROR:
                match self._detail:
                    case _ if self._detail == ERR_UNKNOWN:
                        self._log_rx("error", "[ERROR] Unknown error occurred.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_INVALID_PACKET:
                        self._log_rx("error", "[ERROR] Invalid packet received.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_TIMEOUT:
                        self._log_rx("error", "[ERROR] Operation timed out.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_CHECKSUM:
                        self._log_rx("error", "[ERROR] Checksum error.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_NO_PROCESSOR:
                        self._log_rx("error", "[ERROR] No processor available.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_BUFFER_OVERFLOW:
                        self._log_rx("error", "[ERROR] Buffer overflow.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_INVALID_CMD:
                        self._log_rx("error", "[ERROR] Invalid command.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_INVALID_TELEMETRY_FLAG:
                        self._log_rx("error", "[ERROR] Invalid telemetry flag.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_UNKNOWN_PROGRAM:
                        msg = f"[ERROR] Program: {payload[3].to_bytes(1, 'big').hex()} unknown."
                        self._log_rx("error", msg)
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_INDEX_OOB:
                        self._log_rx("error", "[ERROR] Index out of bounds.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_INVALID_DIR:
                        self._log_rx("error", "[ERROR] Invalid direction.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_JSON_PARSE:
                        self._log_rx("error", "[ERROR] JSON parse error.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_ALLOC_FAIL:
                        self._log_rx("error", "[ERROR] Allocation failed.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)

                    case _ if self._detail == ERR_MISSING_FIELD:
                        self._log_rx("error", "[ERROR] Missing field.")
                        self._log_error_detail(self._cmd_id, self._status, self._detail)
                    case _:
                        print("[ERROR] Unknown STATUS_ERROR detail.")
            case _:
                print("[ERROR] Invalid status.")

    def _validate_packet(self, raw_data: bytes) -> bool:
        # Payload length
        pl_len = raw_data[3]

        payload = raw_data[4 : 4 + pl_len]
        if len(payload) < 3:
            print("Payload too short!")
            return False

        # CRC
        crc_index = 4 + pl_len
        crc_bytes_received = raw_data[crc_index : crc_index + 2]
        crc16_fn = crcmod.predefined.mkCrcFun("modbus")
        crc_value: int = crc16_fn(payload)
        crc_bytes: bytes = crc_value.to_bytes(2, byteorder="little")
        if crc_bytes != crc_bytes_received:
            print("Invalid CRC!")
            return False

        return True

    def _extract_steps(self, data: bytes) -> list[int]:
        values: list[int] = []
        for i in range(0, len(data), 3):
            chunk = data[i : i + 3]
            value = int.from_bytes(chunk, byteorder="big", signed=True)
            values.append(value)
        return values

    def _log_error_detail(self, cmd_id: int, status: int, detail: int):
        print(f"Command ID: 0x{cmd_id:02X}")
        print(f"Status:     0x{status:02X}")
        print(f"Detail:     0x{detail:02X}")
        print("-----------------------------------------------------------------")

    def _log_rx(self, type: str, detail: str):
        status = ""
        if self._status == STATUS_OK:
            status = "OK"
        elif self._status == STATUS_ERROR:
            status = "ERROR"
        else:
            status = "UNKNOWN"

        rx_msg = f"RX: [{status}] :: Robot response received."
        parsed_response_label = detail if detail else f"[{status}]"

        self._ui_manager.update_com_monitor(
            "rx", type, rx_msg, self._raw_data, parsed_response_label
        )
