from app.constants.com_protocol import START_BYTES, END_BYTES


class PacketParser:
    def __init__(self):
        self._buffer = bytearray()

    def feed(self, data: bytes) -> list[bytes]:
        self._buffer.extend(data)
        packets = []

        while True:
            start_idx = self._buffer.find(bytes(START_BYTES))
            if start_idx == -1:
                self._buffer.clear()
                break

            if start_idx > 0:
                self._buffer = self._buffer[start_idx:]

            if len(self._buffer) < 5:
                break

            pl_len = self._buffer[3]
            total_len = 4 + pl_len + 2 + len(END_BYTES)  # hdr + payload + crc + end

            if len(self._buffer) < total_len:
                break

            packet = self._buffer[:total_len]
            self._buffer = self._buffer[total_len:]

            if list(packet[-len(END_BYTES) :]) != END_BYTES:
                print("Invalid end bytes!")
                continue

            packets.append(packet)

        return packets
