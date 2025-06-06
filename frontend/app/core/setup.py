class Setup:
    def __init__(self):
        self._default_val = 0

    # *************************Public Methods****************************
    def save(self):
        pass

    def get_setup(self):
        pass

    # *************************Private Methods****************************
    def _init_dicts_with_def_val(self):
        default_dicts = {
            "dh_params": {
                "joint1": ["theta_offset", "alpha", "d", "a"],
                "joint2": ["theta_offset", "alpha", "d", "a"],
                "joint3": ["theta_offset", "alpha", "d", "a"],
                "joint4": ["theta_offset", "alpha", "d", "a"],
                "joint5": ["theta_offset", "alpha", "d", "a"],
                "joint6": ["theta_offset", "alpha", "d", "a"],
            },
            "homing_params": {
                "motor1": ["vel", "away_vel", "back_vel", "home_pos"],
                "motor2": ["vel", "away_vel", "back_vel", "home_pos"],
                "motor3": ["vel", "away_vel", "back_vel", "home_pos"],
                "motor4": ["vel", "away_vel", "back_vel", "home_pos"],
                "motor5": ["vel", "away_vel", "back_vel", "home_pos"],
                "motor6": ["vel", "away_vel", "back_vel", "home_pos"],
            },
            "speed_a_accel": {
                "motor1": ["max_speed", "acc"],
                "motor2": ["max_speed", "acc"],
                "motor3": ["max_speed", "acc"],
                "motor4": ["max_speed", "acc"],
                "motor5": ["max_speed", "acc"],
                "motor6": ["max_speed", "acc"],
            },
        }

        return {
            key: {
                sub_key: {field: self._default_val for field in val}
                for sub_key, val in values.items()
            }
            for key, values in default_dicts.items()
        }

    def _get_field_vals(self):
        pass

    def _detect_changes(self):
        pass

    def _write_setup_to_file(self):
        try:
            pass
        except:
            pass
        pass

    def _read_setup_from_file(self):
        try:
            pass
        except:
            pass
        pass
