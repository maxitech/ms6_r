import math


class Helper:
    def __init__(self):
        pass

    # *************************Public Methods****************************
    def calc_checksum(self, data):
        checksumVal = 0
        for char in data:
            checksumVal ^= ord(char)

        hex_checksum = format(checksumVal, "X").zfill(2)
        return hex_checksum

    def get_joint_speeds(self, slider_value, s_min, s_max, gear_factors):
        base_speed = self._map_slider_to_speed(slider_value, s_min, s_max)
        return [round(base_speed * math.sqrt(f)) for f in gear_factors]

    # *************************Private Methods****************************
    def _map_slider_to_speed(self, slider_value, v_min, v_max):
        # t = (slider_value - 10) / (100 - 10)  # Normalize slider value from 10 to 100
        slider_value = max(10, slider_value)
        percent = (slider_value - 10) / (100 - 10)
        return v_min + percent * (v_max - v_min)
