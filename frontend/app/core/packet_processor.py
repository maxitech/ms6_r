import crcmod
from app.constants.com_protocol import *


class PacketProcessor:
    def __init__(self):
        self._cmd_id = None
        self._status = None
        self._detail = None

    def process_packet(self, raw_data: bytes):
        if not self._validate_packet(raw_data):
            print("Invalid packet received.")
            return

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
                        pass  # Aktion DATA_NONE
                    case _ if self._detail == DATA_STEPS:
                        pass  # Aktion DATA_STEPS
                    case _ if self._detail == INFO_LOADED_PROGRAM:
                        print(f"[INFO] Program: 0x{payload[3]:02X} loaded.")
                    case _ if self._detail == INFO_RELOADED_PROGRAM:
                        print(f"[INFO] Program: 0x{payload[3]:02X} reloaded.")
                    case _ if self._detail == INFO_ALREADY_RUNNING:
                        print(f"[INFO] Program: 0x{payload[3]:02X} already running.")
                    case _ if self._detail == INFO_HOMING_ALREADY_DONE:
                        print("[INFO] Homing already done.")
                    case _ if self._detail == WARN_NOP_IGNORED:
                        print("[WARN] No operation ignored.")
                    case _ if self._detail == WARN_NO_PROGRAM_LOADED:
                        print("[WARN] No program loaded.")
                    case _ if self._detail == WARN_LIMIT_HIT:
                        print("[WARN] Limit reached.")
                    case _ if self._detail == WARN_ARM_NOT_HOMED:
                        print("[WARN] Arm not homed.")
                    case _:
                        print("[ERROR] Unknown STATUS_OK detail")
            case _ if self._status == STATUS_ERROR:
                match self._detail:
                    case _ if self._detail == ERR_UNKNOWN:
                        print("[ERROR] Unknown error occurred.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_INVALID_PACKET:
                        print("[ERROR] Invalid packet received.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_TIMEOUT:
                        print("[ERROR] Operation timed out.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_CHECKSUM:
                        print("[ERROR] Checksum error.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_NO_PROCESSOR:
                        print("[ERROR] No processor available.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_BUFFER_OVERFLOW:
                        print("[ERROR] Buffer overflow.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_INVALID_CMD:
                        print("[ERROR] Invalid command.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_INVALID_TELEMETRY_FLAG:
                        print("[ERROR] Invalid telemetry flag.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_UNKNOWN_PROGRAM:
                        print(
                            f"[ERROR] Program: {payload[3].to_bytes(1, "big").hex()} unknown."
                        )
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_INDEX_OOB:
                        print("[ERROR] Index out of bounds.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_INVALID_DIR:
                        print("[ERROR] Invalid direction.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_JSON_PARSE:
                        print("[ERROR] JSON parse error.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_ALLOC_FAIL:
                        print("[ERROR] Allocation failed.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
                    case _ if self._detail == ERR_MISSING_FIELD:
                        print("[ERROR] Missing field.")
                        self._logErrorDetail(self._cmd_id, self._status, self._detail)
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

    def _extract_steps(self, data: bytes):
        values = []
        for i in range(0, len(data), 3):
            chunk = data[i : i + 3]
            value = int.from_bytes(chunk, byteorder="big", signed=True)
            values.append(value)
        return values

    def _logErrorDetail(self, cmd_id: int, status: int, detail: int):
        print(f"Command ID: 0x{cmd_id:02X}")
        print(f"Status:     0x{status:02X}")
        print(f"Detail:     0x{detail:02X}")
        print("-----------------------------------------------------------------")
