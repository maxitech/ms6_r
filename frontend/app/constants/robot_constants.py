from dataclasses import dataclass
from typing import List


@dataclass(frozen=True)
class RobotConstants:
    STEPS_PER_REV: List[int]
    MICROSTEPS: int
    MOTOR_SPEED_A_ACCEL: List[List[int]]
    DRIVING_PULLEY_TEETH: List[int]
    DRIVEN_PULLEY_TEETH: List[int]
    HOME_POSITIONS: List[int]
    JOINT_LIMITS: List[List[float]]
    DH_PARAMS: dict
    NAME: str
