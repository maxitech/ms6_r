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
    def _total_ratio():
        # gearboxRatio * (drivenTeeth /driverTeeth)
        pass


if __name__ == "__main__":
    # Debug
    ms6_r = Robot(constants=MS6_R_CONSTANTS)
    q = [0, 0, 0, 0, np.deg2rad(90), 0]
    T = ms6_r.fkine(q=q)
    print(T)
    sol = ms6_r.ikine_LM(T)
    ms6_r.plot(q=q, backend="pyplot", block=True)
