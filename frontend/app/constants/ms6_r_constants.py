from math import pi
from .robot_constants import RobotConstants


MS6_R_CONSTANTS = RobotConstants(
    STEPS_PER_REV=[200, 200, 200, 200, 400, 200],
    MICROSTEPS=64,
    MOTOR_SPEED_A_ACCEL=[
        [50000, 26000],
        [160000, 50000],
        [60000, 30000],
        [40000, 30000],
        [40000, 20000],
        [15000, 8000],
    ],  # max speed, accel
    DRIVER_PULLEY_TEETH=[16, 16, 16, 16, 16, 1],
    DRIVEN_PULLEY_TEETH=[100, 80, 100, 60, 32, 1],
    GEARBOX_RATIO=[1, 14, 1, 1, 1, 1],
    JOINT_DIR=[1, 1, -1, 1, 1, 1],  # direction for each joint
    HOME_POSITIONS=[40250, 42760, -11820, -24080, -16438, -6400],  # steps
    # Min and max positions the joint can reach in degrees after homing
    JOINT_LIMITS=[
        [-180, 152],
        [-14, 88],
        [-57, 50],
        [-166, 180],
        [-120, 110],
        [-174, 180],
    ],  # min,max
    JOINT_JOG_SPEED_RANGE=[500, 4000],  # min,max
    DH_PARAMS={
        # values converted from mm -> m (roboticstoolbox expects SI units)
        "d": [135.30 / 1000.0, 0.0, 0.0, 138.40 / 1000.0, 0.0, 29.27 / 1000.0],
        "a": [37.50 / 1000.0, 160.00 / 1000.0, -15.00 / 1000.0, 0.0, 0.0, 0.0],
        "alpha": [-pi / 2, 0, pi / 2, -pi / 2, pi / 2, 0],
        "offset": [0, -pi / 2, pi, 0, 0, 0],
        # "d": [135.30 / 1000.0, 0.0, 0.0, 138.40 / 1000.0, 0.0, -29.27 / 1000.0],
        # "a": [37.50 / 1000.0, 160.00 / 1000.0, -15.00 / 1000.0, 0.0, 0.0, 0.0],
        # "alpha": [-pi / 2, 0, pi / 2, -pi / 2, pi / 2, 0],
        # "offset": [0, -pi / 2, pi, 0, 0, pi],
    },
    NAME="MS6_R",
)
