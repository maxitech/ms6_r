from dataclasses import dataclass
from typing import List


@dataclass(frozen=True)
class RobotConstants:
    STEPS_PER_REV: List[int]
    MICROSTEPS: int
    MOTOR_SPEED_A_ACCEL: List[List[int]]
    DRIVER_PULLEY_TEETH: List[int]
    DRIVEN_PULLEY_TEETH: List[int]
    GEARBOX_RATIO: List[int]
    HOME_POSITIONS: List[int]
    JOINT_LIMITS: List[List[float]]
    JOINT_JOG_SPEED_RANGE: List[int]
    DH_PARAMS: dict
    NAME: str
