class Helper:
    def __init__(self):
        pass

    def calc_checksum(self, data):
        checksumVal = 0
        for char in data:
            checksumVal ^= ord(char)

        hex_checksum = format(checksumVal, "X").zfill(2)
        return hex_checksum
