import json
import os
from PySide6.QtWidgets import QLineEdit
from app.ui.ui_manager import UIManager
from typing import Union
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
        self._setup = {
            "dh_params": {},
            "homing_params": {},
            "speed_a_accel": {},
        }
        self._extract_values(
            stored_inputs=self._ui.robot_config.dh_inputs,
            prefix="joint",
            setup_part_key="dh_params",
            validate_as=float,
        )
        self._extract_values(
            stored_inputs=self._ui.robot_config.home_pos_inputs,
            prefix="motor",
            setup_part_key="homing_params",
            validate_as=int,
        )
        self._extract_values(
            stored_inputs=self._ui.robot_config.speed_a_accel_inputs,
            prefix="motor",
            setup_part_key="speed_a_accel",
            validate_as=int,
        )

        return self._setup

    def _extract_values(
        self,
        stored_inputs: dict[tuple[int, str], QLineEdit],
        prefix: str,
        setup_part_key: str,
        validate_as: type,
    ):
        prefix = prefix.lower()
        for (row, param), field in stored_inputs.items():
            key = f"{prefix}{row+1}"
            if key not in self._setup[setup_part_key]:
                self._setup[setup_part_key][key] = {}

            value = field.text().strip()
            self._field = param  # For error reporting

            if not value:
                print(f"ERROR: Field '{self._field}' cannot be empty.")
                self._is_input_valid = False
                return

            try:
                validate_as(value)
                self._setup[setup_part_key][key][param] = value
                self._is_input_valid = True
            except Exception as e:
                print(f"ERROR: Field '{self._field}' must be a numeric value.")
                self._handle_exception(e, "_extract_values")
                self._is_input_valid = False
                return

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
            self._update_table(
                setup_part=setup.get("dh_params", {}),
                prefix="joint",
                stored_inputs=self._ui.robot_config.dh_inputs,
            )
            self._update_table(
                setup_part=setup.get("homing_params", {}),
                prefix="motor",
                stored_inputs=self._ui.robot_config.home_pos_inputs,
            )
            self._update_table(
                setup_part=setup.get("speed_a_accel", {}),
                prefix="motor",
                stored_inputs=self._ui.robot_config.speed_a_accel_inputs,
            )

    def _update_table(
        self,
        setup_part: dict[str, dict[str, Union[int, float]]],
        prefix: str,
        stored_inputs: dict[tuple[int, str], QLineEdit],
    ):
        prefix = prefix.lower()

        for field_name, param_data in setup_part.items():
            if field_name.startswith(prefix):
                row = int(field_name.replace(prefix, "")) - 1
                for param_key, value in param_data.items():
                    field = stored_inputs.get((row, param_key))
                    if field:
                        field.setText(str(value))
