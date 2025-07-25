import roboticstoolbox as rtb
from roboticstoolbox import *
from math import pi
import numpy as np
import matplotlib.pyplot as plt


class RobotConstants:
    STEPS_PER_REV = [200, 200, 200, 200, 400, 200]
    MICRO_STEPS = 64
    MOTOR_SPEED_A_ACCEL = [
        [50000, 26000],
        [160000, 50000],
        [60000, 30000],
        [40000, 30000],
        [40000, 20000],
        [15000, 8000],
    ]  # max speed, accel
    DRIVING_PULLEY_TEETH = [16, 16, 16, 16, 16, 1]
    DRIVEN_PULLEY_TEETH = [100, 80, 100, 60, 32, 1]
    HOME_POSITIONS = [40250, 42760, -11820, -24080, -16438, -6400]  # steps

    # Min and max positions the joint can reach in degrees after homing
    JOINT_LIMTS = [
        [-180, 152],
        [-14, 88],
        [-57, 50],
        [-166, 180],
        [-120, 110],
        [-174, 180],
    ]  # min,max

    DH_PARAMS = {
        "d": [135.30, 0, 0, 138.40, 0, 29.27],
        "a": [37.50, 160.00, -15.00, 0, 0, 0],
        "alpha": [-pi / 2, 0, pi / 2, -pi / 2, pi / 2, 0],
        "offset": [0, -pi / 2, pi, 0, 0, 0],
    }
    NAME = "MS6_R"


class Robot:
    def __init__(self):
        dh = RobotConstants.DH_PARAMS
        self.robot = DHRobot(
            [
                RevoluteDH(
                    d=dh["d"][i],
                    a=dh["a"][i],
                    alpha=dh["alpha"][i],
                    offset=dh["offset"][i],
                )
                for i in range(6)
            ],
            name=RobotConstants.NAME,
        )


# if __name__ == "__main__":
#     rbt_instance = Robot()
#     q = [0, 0, 0, 0, np.deg2rad(90), 0]
#     T = rbt_instance.robot.fkine(q=q)
#     print(T)
#     sol = rbt_instance.robot.ikine_LM(T)
#     rbt_instance.robot.plot(q=q, backend="pyplot", block=True)
# pass
