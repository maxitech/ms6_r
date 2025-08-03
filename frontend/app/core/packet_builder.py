from app.constants.com_protocoll import *
from typing import List, Callable, Any


class PacketBuilder:
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
        print(packet)
        return packet

    def _int_to_3_bytes(self, val: int) -> bytes:
        val = val & 0xFFFFFF  # keep only lower 24 bits for 3-byte representation
        return val.to_bytes(length=3, byteorder="big", signed=True)

    def _pack_jog_speeds(self, speeds: List[int]) -> bytes:
        payload = bytearray()
        for val in speeds:
            payload.extend(self._int_to_3_bytes(val))
        return bytes(payload)
