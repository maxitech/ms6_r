import roboticstoolbox as rtb
from roboticstoolbox import *
from math import pi
import numpy as np
import matplotlib.pyplot as plt

from app.constants.ms6_r_constants import RobotConstants
from app.constants.ms6_r_constants import MS6_R_CONSTANTS


class Robot(DHRobot):
    def __init__(self, constants: RobotConstants):
        dh = constants.DH_PARAMS
        super().__init__(
            [
                RevoluteDH(
                    d=dh["d"][i],
                    a=dh["a"][i],
                    alpha=dh["alpha"][i],
                    offset=dh["offset"][i],
                )
                for i in range(6)
            ],
            name=constants.NAME,
        )

        self.constants = constants

    # *** Public ***
    # *** Private ***
    def _total_ratio(self, joint_index: int) -> float:
        c = self.constants
        return c.GEARBOX_RATIO[joint_index] * (
            c.DRIVEN_PULLEY_TEETH[joint_index] / c.DRIVER_PULLEY_TEETH[joint_index]
        )

    def _steps_per_rev(self, joint_index: int) -> float:
        c = self.constants
        return (
            c.STEPS_PER_REV[joint_index]
            * c.MICROSTEPS
            * self._total_ratio(joint_index=joint_index)
        )

    def _steps_2_deg(self, joint_index: int, curr_steps: int) -> float:
        c = self.constants
        home_offset: int = c.HOME_POSITIONS[joint_index]
        relative_steps: int = curr_steps - home_offset
        return (
            float(relative_steps) / self._steps_per_rev(joint_index=joint_index) * 360
        )

    def _deg_2_steps(self, joint_index: int, deg: float) -> int:
        return int((deg / 360) * self._steps_per_rev(joint_index=joint_index))


if __name__ == "__main__":
    # Debug
    ms6_r = Robot(constants=MS6_R_CONSTANTS)
    q = [0, 0, 0, 0, np.deg2rad(90), 0]
    T = ms6_r.fkine(q=q)
    print(T)
    sol = ms6_r.ikine_LM(T)
    ms6_r.plot(q=q, backend="pyplot", block=True)
