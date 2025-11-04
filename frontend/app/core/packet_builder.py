import crcmod
from app.constants.com_protocol import *
from typing import List, Callable, Any


class PacketBuilder:
    SIGNED_24BIT_MIN = -(1 << 23)  # -8388608
    SIGNED_24BIT_MAX = (1 << 23) - 1  # 8388607

    def __init__(self):
        self.payload_map = {
            CMD_JOG: self._pack_jog_speeds,
            CMD_LOAD: self._pack_load_program,
            CMD_START: self._pack_start_exe,
            CMD_STOP: self._pack_stop_exe,
            CMD_IDLE: self._pack_idle_exe,
        }

    def build_packet(self, cmd_id: int, data) -> bytes:
        if not 0 <= cmd_id <= 255:
            raise ValueError("cmd_id must be 0-255")
        pack_fn: Callable[[Any], bytes] | None = self.payload_map.get(cmd_id)
        if not pack_fn:
            raise ValueError(f"Unknown command ID: '{cmd_id}'")
        dynamic_payload: bytes = pack_fn(data)
        fix_payload: bytes = NOP.to_bytes(1, "big", signed=False)
        fpl_len = len(fix_payload).to_bytes(1, "big", signed=False)
        payload: bytes = (
            cmd_id.to_bytes(1, "big", signed=False)
            + dynamic_payload
            + fix_payload
            + fpl_len
        )
        pl_len: bytes = len(payload).to_bytes(1, "big", signed=False)
        crc16_fn = crcmod.predefined.mkCrcFun("modbus")
        crc_value: int = crc16_fn(payload)
        crc_bytes: bytes = crc_value.to_bytes(2, byteorder="little")

        packet = bytes(START_BYTES) + pl_len + payload + crc_bytes + bytes(END_BYTES)
        return packet

    def _pack_load_program(self, prog: int) -> bytes:
        return bytes([prog])

    def _pack_start_exe(self, data: int) -> bytes:
        return bytes([data])

    def _pack_stop_exe(self, data: int) -> bytes:
        return bytes([data])

    def _pack_idle_exe(self, data: int) -> bytes:
        return bytes([data])

    def _int_to_3_bytes(self, val: int) -> bytes:
        if not self.SIGNED_24BIT_MIN <= val <= self.SIGNED_24BIT_MAX:
            raise OverflowError(
                f"Value {val} out of range for signed 24-bit representation"
            )
        return val.to_bytes(length=3, byteorder="big", signed=True)

    def _pack_jog_speeds(self, speeds: List[int]) -> bytes:
        payload = bytearray()
        for val in speeds:
            payload.extend(self._int_to_3_bytes(val))
        return bytes(payload)


packet_builder = PacketBuilder()
