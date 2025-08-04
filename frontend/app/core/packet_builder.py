from app.constants.com_protocoll import *
from typing import List, Callable, Any


class PacketBuilder:
    SIGNED_24BIT_MIN = -(1 << 23)  # -8388608
    SIGNED_24BIT_MAX = (1 << 23) - 1  # 8388607

    def __init__(self):
        self.payload_map = {
            CMD_JOG: self._pack_jog_speeds,
        }

    def build_packet(self, cmd_id: int, data) -> bytes:
        pack_fn: Callable[[Any], bytes] | None = self.payload_map.get(cmd_id)
        if not pack_fn:
            raise ValueError(f"Unknown command ID: '{cmd_id}'")
        dynamic_payload: bytes = pack_fn(data)
        fix_payload: bytes = bytes([NOP])
        payload: bytes = bytes([cmd_id]) + dynamic_payload + fix_payload
        p_len: int = len(payload)
        crc: int = 0

        packet = (
            bytes(START_BYTES)
            + bytes([p_len])
            + payload
            + bytes([crc])
            + bytes(END_BYTES)
        )
        return packet

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
