import roboticstoolbox as rtb
from roboticstoolbox import *
from math import pi
import numpy as np
import matplotlib.pyplot as plt

STEPS_PER_REV = [200, 200, 200, 200, 400, 200]
MICRO_STEPS = 64
DRIVING_PULLEY_TEETH = [16, 16, 16, 16, 16, 1]
DRIVEN_PULLEY_TEETH = [100, 80, 100, 60, 32, 1]
HOME_POSITIONS = [0, 0, 0, 0, 0, 0]


# Min and max positions the joint can reach in degrees after homing
JOINT_LIMTS = [
    [-180, 152],
    [-14, 88],
    [-57, 50],
    [-166, 180],
    [-120, 110],
    [-174, 180],
]  # min,max


d = [135.30, 0, 0, 138.40, 0, 29.27]
a = [37.50, 160.00, -15.00, 0, 0, 0]
alpha = [-pi / 2, 0, pi / 2, -pi / 2, pi / 2, 0]
t_offset = [0, -pi / 2, pi, 0, 0, 0]


robot = DHRobot(
    [
        RevoluteDH(d=d[0], a=a[0], alpha=alpha[0], offset=t_offset[0]),
        RevoluteDH(d=d[1], a=a[1], alpha=alpha[1], offset=t_offset[1]),
        RevoluteDH(d=d[2], a=a[2], alpha=alpha[2], offset=t_offset[2]),
        RevoluteDH(d=d[3], a=a[3], alpha=alpha[3], offset=t_offset[3]),
        RevoluteDH(d=d[4], a=a[4], alpha=alpha[4], offset=t_offset[4]),
        RevoluteDH(d=d[5], a=a[5], alpha=alpha[5], offset=t_offset[5]),
    ],
    name="MS6_R",
)

if __name__ == "__main__":
    # q = [0, 0, 0, 0, np.deg2rad(90), 0]
    # T = robot.fkine(q=q)
    # print(T)
    # sol = robot.ikine_LM(T)
    # robot.plot(q=q, backend="pyplot", block=True)
