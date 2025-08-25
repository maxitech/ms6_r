import crcmod
from app.constants.com_protocol import *


class PacketProcessor:
    def __init__(self):
        self._status = None
        self._detail = None

    def process_packet(self, raw_data: bytes):
        if not self._validate_packet(raw_data):
            print("Invalid packet received.")
            return

        # extract data
        pl_len = raw_data[3]
        payload = raw_data[4 : 4 + pl_len]

        cmd_id = payload[0]
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
                        print("INFO_LOADED_PROGRAM")
                        pass
                    case _ if self._detail == INFO_RELOADED_PROGRAM:
                        print("INFO_RELOADED_PROGRAM")
                        pass
                    case _ if self._detail == INFO_ALREADY_RUNNING:
                        pass
                    case _ if self._detail == INFO_HOMING_ALREADY_DONE:
                        pass
                    case _ if self._detail == WARN_NOP_IGNORED:
                        pass
                    case _ if self._detail == WARN_NO_PROGRAM_LOADED:
                        pass
                    case _ if self._detail == WARN_LIMIT_HIT:
                        pass
                    case _ if self._detail == WARN_ARM_NOT_HOMED:
                        pass
                    case _:
                        pass  # Unbekannter STATUS_OK-Detail
            case _ if self._status == STATUS_ERROR:
                match self._detail:
                    case _ if self._detail == ERR_UNKNOWN:
                        pass
                    case _ if self._detail == ERR_INVALID_PACKET:
                        pass
                    case _ if self._detail == ERR_TIMEOUT:
                        pass
                    case _ if self._detail == ERR_CHECKSUM:
                        pass
                    case _ if self._detail == ERR_NO_PROCESSOR:
                        pass
                    case _ if self._detail == ERR_BUFFER_OVERFLOW:
                        pass
                    case _ if self._detail == ERR_INVALID_CMD:
                        pass
                    case _ if self._detail == ERR_INVALID_TELEMETRY_FLAG:
                        pass
                    case _ if self._detail == ERR_UNKNOWN_PROGRAM:
                        pass
                    case _ if self._detail == ERR_INDEX_OOB:
                        pass
                    case _ if self._detail == ERR_INVALID_DIR:
                        pass
                    case _ if self._detail == ERR_JSON_PARSE:
                        pass
                    case _ if self._detail == ERR_ALLOC_FAIL:
                        pass
                    case _ if self._detail == ERR_MISSING_FIELD:
                        pass
                    case _:
                        pass  # Unbekannter STATUS_ERROR-Detail
            case _:
                pass  # Ungültiger Status

    def _validate_packet(self, raw_data: bytes) -> bool:
        if list(raw_data[:3]) != START_BYTES:
            print("Invalid start bytes!")
            return False

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

        # End bytes
        end_bytes = raw_data[crc_index + 2 :]
        if list(end_bytes) != END_BYTES:
            print("Invalid end bytes!")
            return False

        return True
