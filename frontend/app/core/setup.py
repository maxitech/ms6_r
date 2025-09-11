import json
import os
from app.ui.ui_manager import UIManager
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from main import MainWindow


class Setup:
    def __init__(self, ui_manager: UIManager, ui: "MainWindow"):
        self._ui_manager = ui_manager
        self._ui = ui
        self._default_val = "0"
        self._curr_dir = os.path.dirname(os.path.abspath(__file__))
        self._parent_dir = os.path.dirname(self._curr_dir)
        self._folder_path = os.path.join(self._parent_dir, "setup_gen")
        self._file_path = os.path.join(self._folder_path, "setup.json")
        self._current_setup = self._load_or_init_setup()
        self._is_input_valid = False
        self._update_ui()

    # *************************Public Methods****************************
    def save(self):
        self._write_setup_to_file()

    def get_setup(self):
        return self._read_setup_from_file()

    # *************************Private Methods****************************
    def _load_or_init_setup(self):
        if not os.path.exists(self._file_path):
            self._ui_manager.update_com_monitor("sys", "info", "No setup file found.")
            print(f"No setup file found.")
            return self._init_dicts_with_def_val()
        else:
            self._ui_manager.update_com_monitor(
                "sys", "info", "Loading setup from file."
            )
            print("Loading setup from file.")
            return self.get_setup()

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

        default_setup = {
            key: {
                sub_key: {field: self._default_val for field in val}
                for sub_key, val in values.items()
            }
            for key, values in default_dicts.items()
        }
        self._current_setup = default_setup
        self._write_setup_to_file()

    def _get_field_vals(self):
        if not self._ui:
            return
        setup = {
            "dh_params": {},
            # "homing_params": {
            #     "motor1": {
            #         # "vel": self._ui.home_mtr_1_vel.toPlainText(),
            #         # "away_vel": self._ui.home_mtr_1_aw_vel.toPlainText(),
            #         # "back_vel": self._ui.home_mtr_1_bck_vel.toPlainText(),
            #         "home_pos": self._ui.home_mtr_1_h_pos.toPlainText(),
            #     },
            #     "motor2": {
            #         # "vel": self._ui.home_mtr_2_vel.toPlainText(),
            #         # "away_vel": self._ui.home_mtr_2_aw_vel.toPlainText(),
            #         # "back_vel": self._ui.home_mtr_2_bck_vel.toPlainText(),
            #         "home_pos": self._ui.home_mtr_2_h_pos.toPlainText(),
            #     },
            #     "motor3": {
            #         # "vel": self._ui.home_mtr_3_vel.toPlainText(),
            #         # "away_vel": self._ui.home_mtr_3_aw_vel.toPlainText(),
            #         # "back_vel": self._ui.home_mtr_3_bck_vel.toPlainText(),
            #         "home_pos": self._ui.home_mtr_3_h_pos.toPlainText(),
            #     },
            #     "motor4": {
            #         # "vel": self._ui.home_mtr_4_vel.toPlainText(),
            #         # "away_vel": self._ui.home_mtr_4_aw_vel.toPlainText(),
            #         # "back_vel": self._ui.home_mtr_4_bck_vel.toPlainText(),
            #         "home_pos": self._ui.home_mtr_4_h_pos.toPlainText(),
            #     },
            #     "motor5": {
            #         # "vel": self._ui.home_mtr_5_vel.toPlainText(),
            #         # "away_vel": self._ui.home_mtr_5_aw_vel.toPlainText(),
            #         # "back_vel": self._ui.home_mtr_5_bck_vel.toPlainText(),
            #         "home_pos": self._ui.home_mtr_5_h_pos.toPlainText(),
            #     },
            #     "motor6": {
            #         # "vel": self._ui.home_mtr_6_vel.toPlainText(),
            #         # "away_vel": self._ui.home_mtr_6_aw_vel.toPlainText(),
            #         # "back_vel": self._ui.home_mtr_6_bck_vel.toPlainText(),
            #         "home_pos": self._ui.home_mtr_6_h_pos.toPlainText(),
            #     },
            # },
            # "speed_a_accel": {
            #     "motor1": {
            #         "max_speed": self._ui.motor_1_max_speed.toPlainText(),
            #         "acc": self._ui.motor_1_acc.toPlainText(),
            #     },
            #     "motor2": {
            #         "max_speed": self._ui.motor_2_max_speed.toPlainText(),
            #         "acc": self._ui.motor_2_acc.toPlainText(),
            #     },
            #     "motor3": {
            #         "max_speed": self._ui.motor_3_max_speed.toPlainText(),
            #         "acc": self._ui.motor_3_acc.toPlainText(),
            #     },
            #     "motor4": {
            #         "max_speed": self._ui.motor_4_max_speed.toPlainText(),
            #         "acc": self._ui.motor_4_acc.toPlainText(),
            #     },
            #     "motor5": {
            #         "max_speed": self._ui.motor_5_max_speed.toPlainText(),
            #         "acc": self._ui.motor_5_acc.toPlainText(),
            #     },
            #     "motor6": {
            #         "max_speed": self._ui.motor_6_max_speed.toPlainText(),
            #         "acc": self._ui.motor_6_acc.toPlainText(),
            #     },
            # },
        }
        for (row, param), field in self._ui.robot_config.dh_inputs.items():
            joint_key = f"joint{row+1}"
            if joint_key not in setup["dh_params"]:
                setup["dh_params"][joint_key] = {}

            value = field.text().strip()
            self._field = param  # For error reporting

            if not value:
                print(f"ERROR: Field '{self._field}' cannot be empty.")
                self._is_input_valid = False
                return

            try:
                float(value)
                setup["dh_params"][joint_key][param] = value
                self._is_input_valid = True
            except Exception as e:
                print(f"ERROR: Field '{self._field}' must be a numeric value.")
                self._handle_exception(e, "_get_field_vals")
                self._is_input_valid = False
                return
        return setup

    def _detect_changes(self, current_setup, previous_setup):
        return json.dumps(current_setup, sort_keys=True) != json.dumps(
            previous_setup, sort_keys=True
        )

    def _write_setup_to_file(self):
        try:
            if not os.path.exists(self._folder_path):
                print(f"Creating setup folder at {self._folder_path}.")
                os.makedirs(self._folder_path)
            if not os.path.exists(self._file_path):
                print(f"Creating setup file at {self._file_path}.")
                with open(self._file_path, "w") as f:
                    print(f"Initializing setup file with defaults.")
                    json.dump(self._current_setup, f, indent=4)
            else:
                received_current_setup = self._get_field_vals()
                if self._is_input_valid and self._detect_changes(
                    received_current_setup, self._current_setup
                ):
                    self._current_setup = received_current_setup
                    print("Changes detected, updating setup file.")
                    with open(self._file_path, "w") as f:
                        json.dump(self._current_setup, f, indent=4)
        except Exception as e:
            self._handle_exception(e, "_write_setup_to_file")

    def _read_setup_from_file(self):
        try:
            if os.path.exists(self._file_path):
                with open(self._file_path, "r") as f:
                    return json.load(f)
        except Exception as e:
            self._handle_exception(e, "_read_setup_from_file")
            return self._init_dicts_with_def_val()

    def _handle_exception(self, e, context):
        if isinstance(e, FileNotFoundError):
            print(f"ERROR: File not found in {context}: {self._file_path}.")
        elif isinstance(e, PermissionError):
            print(f"ERROR: Permission denied in {context}: {self._file_path}.")
        elif isinstance(e, json.JSONDecodeError):
            print(f"ERROR: JSON decode error in {context}: {self._file_path}.")
        elif isinstance(e, ValueError):
            print(f"ERROR: Value error in {context}: {e}.")
        else:
            print(f"ERROR: Unexpected error in {context}: {e}")

    def _update_ui(self):
        if not self._ui:
            return
        setup = self.get_setup()
        if setup:
            self._update_dh_table(setup.get("dh_params", {}))

    def _update_dh_table(self, dh_params: dict[str, dict[str, float]]):
        for joint_name, param_data in dh_params.items():
            if joint_name.startswith("joint"):
                row = int(joint_name.replace("joint", "")) - 1
                for param_key, value in param_data.items():
                    field = self._ui.robot_config.dh_inputs.get((row, param_key))
                    if field:
                        field.setText(str(value))
