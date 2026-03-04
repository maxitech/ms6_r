import roboticstoolbox as rtb
from roboticstoolbox import DHRobot, RevoluteDH

from math import pi
import numpy as np
import numpy.typing as npt
import matplotlib.pyplot as plt
from typing import List

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
    def get_joint_angles_deg(self, curr_steps: List[int]) -> List[float]:
        angles_deg: List[float] = []
        for i in range(len(curr_steps)):
            deg = self.steps_2_deg(i, curr_steps=curr_steps[i])
            angles_deg.append(deg)
        return angles_deg

    # def deg_2_steps(self, joint_index: int, deg: float) -> int:
    #     return int((deg / 360) * self._steps_per_rev(joint_index=joint_index))

    def deg_2_steps(
        self, joint_index: int, deg: float, apply_home_offset: bool = True
    ) -> int:
        """Umrechnung von Grad zu Steps"""
        steps = int((deg / 360) * self._steps_per_rev(joint_index=joint_index))
        if apply_home_offset:
            steps += self.constants.HOME_POSITIONS[joint_index]
        return steps

    # def steps_2_deg(self, joint_index: int, curr_steps: int) -> float:
    #     c = self.constants
    #     home_offset: int = c.HOME_POSITIONS[joint_index]
    #     relative_steps: int = curr_steps - home_offset
    #     return (
    #         float(relative_steps) / self._steps_per_rev(joint_index=joint_index) * 360
    #     )

    def steps_2_deg(
        self, joint_index: int, curr_steps: int, remove_home_offset: bool = True
    ) -> float:
        """Umrechnung von Steps zu Grad"""
        if remove_home_offset:
            relative_steps = curr_steps - self.constants.HOME_POSITIONS[joint_index]
        else:
            relative_steps = curr_steps

        return (
            float(relative_steps) / self._steps_per_rev(joint_index=joint_index) * 360
        )

    # def steps_to_q_rad(self, steps: list[int]) -> npt.NDArray[np.float64]:
    #     """Konvertiert Steps zu Joint angles in Radians."""
    #     degs: list[float] = self.get_joint_angles_deg(steps)

    #     #  Adjust the sign according to the mechanical installation direction.
    #     dirs = np.array(self.constants.JOINT_DIR, dtype=float)
    #     degs = degs * dirs

    #     return np.deg2rad(degs)

    def steps_to_q_rad(
        self, steps: list[int], remove_home_offset: bool = True
    ) -> npt.NDArray[np.float64]:
        """Konvertiert Steps zu Joint angles in Radians."""
        degs = []
        for i in range(6):
            degs.append(self.steps_2_deg(i, steps[i], remove_home_offset))

        # Adjust the sign according to the mechanical installation direction.
        dirs = np.array(self.constants.JOINT_DIR, dtype=float)
        degs = degs * dirs

        return np.deg2rad(degs)

    def q_rad_to_steps(
        self, q_rad: npt.NDArray[np.float64], apply_home_offset: bool = True
    ) -> List[int]:
        """Konvertiert Joint angles in Radians zu Steps."""
        degs = np.rad2deg(q_rad)

        # Reverse the joint direction adjustment
        dirs = np.array(self.constants.JOINT_DIR, dtype=float)
        degs = degs * dirs

        steps = []
        for i in range(6):
            steps.append(self.deg_2_steps(i, degs[i], apply_home_offset))

        return steps

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

    def _get_joint_limits_in_steps(self) -> List[List[int]]:
        step_limits = []
        for i, (min_deg, max_deg) in enumerate(self.constants.JOINT_LIMITS):
            min_steps = self.deg_2_steps(i, min_deg)
            max_steps = self.deg_2_steps(i, max_deg)
            step_limits.append([int(min_steps), int(max_steps)])
        return step_limits


if __name__ == "__main__":
    pass
    # Debug
    # ms6_r = Robot(constants=MS6_R_CONSTANTS)
    # q = [0, 0, 0, 0, np.deg2rad(90), 0]
    # T = ms6_r.fkine(q=q)
    # print("FK pos:")
    # print(T)
    # print("------------------------------------------------")
    # sol = ms6_r.ikine_LM(T)
    # ms6_r.plot(q=q, backend="pyplot", block=True)
    # print("Steps for j1: ", ms6_r._deg_2_steps(0, 180))
    # print("------------------------------------------------")
    # print("Joint angles in deg: ")
    # TEST_POSITIONS = [40250 + 40000, 42760, -11820, -24080, -16438, -6400 * 2]  # steps
    # for angle in ms6_r.get_joint_angles_deg(TEST_POSITIONS):
    #     print(angle)
    # print("------------------------------------------------")
    # print("Joint limits in steps: ")
    # print(ms6_r._get_joint_limits_in_steps())
    # print("------------------------------------------------")
