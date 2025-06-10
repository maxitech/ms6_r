import json
import os


class Setup:
    def __init__(self, ui=None):
        self._ui = ui
        self._default_val = 0
        self._curr_dir = os.path.dirname(os.path.abspath(__file__))
        self._parent_dir = os.path.dirname(self._curr_dir)
        self._folder_path = os.path.join(self._parent_dir, "setup_gen")
        self._file_path = os.path.join(self._folder_path, "setup.json")

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
        if not self._ui:
            return
        setup = {
            "dh_params": {
                "joint1": {
                    "theta_offset": self._ui.dh_j1_theta_off.toPlainText(),
                    "alpha": self._ui.dh_j1_aph_v.toPlainText(),
                    "d": self._ui.dh_j1_d_v.toPlainText(),
                    "a": self._ui.dh_j1_a_v.toPlainText(),
                },
                "joint2": {
                    "theta_offset": self._ui.dh_j2_theta_off.toPlainText(),
                    "alpha": self._ui.dh_j2_aph_v.toPlainText(),
                    "d": self._ui.dh_j2_d_v.toPlainText(),
                    "a": self._ui.dh_j2_a_v.toPlainText(),
                },
                "joint3": {
                    "theta_offset": self._ui.dh_j3_theta_off.toPlainText(),
                    "alpha": self._ui.dh_j3_aph_v.toPlainText(),
                    "d": self._ui.dh_j3_d_v.toPlainText(),
                    "a": self._ui.dh_j3_a_v.toPlainText(),
                },
                "joint4": {
                    "theta_offset": self._ui.dh_j4_theta_off.toPlainText(),
                    "alpha": self._ui.dh_j4_aph_v.toPlainText(),
                    "d": self._ui.dh_j4_d_v.toPlainText(),
                    "a": self._ui.dh_j4_a_v.toPlainText(),
                },
                "joint5": {
                    "theta_offset": self._ui.dh_j5_theta_off.toPlainText(),
                    "alpha": self._ui.dh_j5_aph_v.toPlainText(),
                    "d": self._ui.dh_j5_d_v.toPlainText(),
                    "a": self._ui.dh_j5_a_v.toPlainText(),
                },
                "joint6": {
                    "theta_offset": self._ui.dh_j6_theta_off.toPlainText(),
                    "alpha": self._ui.dh_j6_aph_v.toPlainText(),
                    "d": self._ui.dh_j6_d_v.toPlainText(),
                    "a": self._ui.dh_j6_a_v.toPlainText(),
                },
            },
            "homing_params": {
                "motor1": {
                    "vel": self._ui.home_mtr_1_vel.toPlainText(),
                    "away_vel": self._ui.home_mtr_1_aw_vel.toPlainText(),
                    "back_vel": self._ui.home_mtr_1_bck_vel.toPlainText(),
                    "home_pos": self._ui.home_mtr_1_h_pos.toPlainText(),
                },
                "motor2": {
                    "vel": self._ui.home_mtr_2_vel.toPlainText(),
                    "away_vel": self._ui.home_mtr_2_aw_vel.toPlainText(),
                    "back_vel": self._ui.home_mtr_2_bck_vel.toPlainText(),
                    "home_pos": self._ui.home_mtr_2_h_pos.toPlainText(),
                },
                "motor3": {
                    "vel": self._ui.home_mtr_3_vel.toPlainText(),
                    "away_vel": self._ui.home_mtr_3_aw_vel.toPlainText(),
                    "back_vel": self._ui.home_mtr_3_bck_vel.toPlainText(),
                    "home_pos": self._ui.home_mtr_3_h_pos.toPlainText(),
                },
                "motor4": {
                    "vel": self._ui.home_mtr_4_vel.toPlainText(),
                    "away_vel": self._ui.home_mtr_4_aw_vel.toPlainText(),
                    "back_vel": self._ui.home_mtr_4_bck_vel.toPlainText(),
                    "home_pos": self._ui.home_mtr_4_h_pos.toPlainText(),
                },
                "motor5": {
                    "vel": self._ui.home_mtr_5_vel.toPlainText(),
                    "away_vel": self._ui.home_mtr_5_aw_vel.toPlainText(),
                    "back_vel": self._ui.home_mtr_5_bck_vel.toPlainText(),
                    "home_pos": self._ui.home_mtr_5_h_pos.toPlainText(),
                },
                "motor6": {
                    "vel": self._ui.home_mtr_6_vel.toPlainText(),
                    "away_vel": self._ui.home_mtr_6_aw_vel.toPlainText(),
                    "back_vel": self._ui.home_mtr_6_bck_vel.toPlainText(),
                    "home_pos": self._ui.home_mtr_6_h_pos.toPlainText(),
                },
            },
            "speed_a_accel": {
                "motor1": {
                    "max_speed": self._ui.motor_1_max_speed.toPlainText(),
                    "acc": self._ui.motor_1_acc.toPlainText(),
                },
                "motor2": {
                    "max_speed": self._ui.motor_2_max_speed.toPlainText(),
                    "acc": self._ui.motor_2_acc.toPlainText(),
                },
                "motor3": {
                    "max_speed": self._ui.motor_3_max_speed.toPlainText(),
                    "acc": self._ui.motor_3_acc.toPlainText(),
                },
                "motor4": {
                    "max_speed": self._ui.motor_4_max_speed.toPlainText(),
                    "acc": self._ui.motor_4_acc.toPlainText(),
                },
                "motor5": {
                    "max_speed": self._ui.motor_5_max_speed.toPlainText(),
                    "acc": self._ui.motor_5_acc.toPlainText(),
                },
                "motor6": {
                    "max_speed": self._ui.motor_6_max_speed.toPlainText(),
                    "acc": self._ui.motor_6_acc.toPlainText(),
                },
            },
        }
        for _, value in setup.items():
            for _, sub_value in value.items():
                for field, field_value in sub_value.items():
                    self._field = (
                        field  # to ensure the field is accessible in the exception
                    )
                    checked_value = field_value.strip()
                    if not checked_value:
                        print(f"ERROR: Field '{self._field}' cannot be empty.")
                    try:
                        float(checked_value)
                    except ValueError:
                        print(f"ERROR: Field '{self._field}' must be a numeric value.")
                    sub_value[field] = checked_value

        return setup

    def _detect_changes(self, current_setup, previous_setup):
        if not current_setup == previous_setup:
            return True
        return False

    def _write_setup_to_file(self):
        try:
            if not os.path.exists(self._folder_path):
                os.makedirs(self._folder_path)
            if not os.path.exists(self._file_path):
                with open(self._file_path, "w") as f:
                    json.dump(self._init_dicts_with_def_val(), f, indent=4)
            else:
                with open(self._file_path, "w") as f:
                    # write current setup values to file
                    pass
        except:
            pass

    def _read_setup_from_file(self):
        try:
            pass
        except:
            pass
        pass
