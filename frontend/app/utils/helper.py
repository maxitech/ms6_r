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

    def get_joint_speeds(self, slider_value, v_min, v_max, gear_factors):
        base_speed = self._map_slider_to_speed(slider_value, v_min, v_max)
        return [round(base_speed * math.sqrt(f)) for f in gear_factors]

    def map_slider_to_linear_speed(self, slider_val, min_mm_s=1.0, max_mm_s=100.0):
        percent = slider_val / 100  # 100 max_slider_val
        return min_mm_s + percent * (max_mm_s - min_mm_s)

    def map_slider_to_rot_speed(self, slider_val, min_deg_s=1.0, max_deg_s=100.0):
        percent = slider_val / 100
        return min_deg_s + percent * (max_deg_s - min_deg_s)

    # *************************Private Methods****************************
    def _map_slider_to_speed(self, slider_value, v_min, v_max):
        percent = slider_value / 100
        return v_min + percent * (v_max - v_min)
