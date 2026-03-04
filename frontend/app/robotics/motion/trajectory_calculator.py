# import numpy as np
# from roboticstoolbox import jtraj, ctraj
# from math import pi
# from typing import cast, List, Tuple
# import struct
# from spatialmath import SE3
# from spatialmath.base import rpy2r
# from app.robotics.robot import Robot


# class TrajectoryCalculator:
#     """Synchronisierte Trajektorienberechnung mit fester 100Hz Frequenz"""

#     def __init__(self, robot: Robot):
#         self.robot = robot
#         self.CONTROL_RATE_HZ = 100  # FESTE Frequenz für alles!
#         self.CONTROL_PERIOD_MS = 10  # 10ms pro Punkt

#     def calculate_joint_trajectory(
#         self,
#         start_steps: List[int],
#         target_steps: List[int],
#         move_time: float = 5.0,
#     ) -> Tuple[List[Tuple[int, List[int]]], int]:
#         """
#         Berechnet Joint-Trajektorie mit FESTER 100Hz Frequenz
#         Gibt zurück: (Punkte mit Sequenznummern, Gesamtdauer_ms)
#         """
#         # Garantiere minimale/maximale Zeit
#         move_time = max(0.5, min(move_time, 30.0))

#         # Anzahl Punkte basierend auf 100Hz
#         num_points = int(move_time * self.CONTROL_RATE_HZ)
#         num_points = max(10, min(num_points, 1000))

#         # Gesamtdauer berechnen (muss zu 100Hz passen!)
#         duration_ms = int(num_points * self.CONTROL_PERIOD_MS)
#         move_time = duration_ms / 1000.0  # Synchronisierte Zeit

#         # Steps -> Radians
#         start_rad = self.robot.steps_to_q_rad(start_steps, remove_home_offset=True)
#         target_rad = self.robot.steps_to_q_rad(target_steps, remove_home_offset=True)

#         # Trajektorie berechnen
#         traj = jtraj(start_rad, target_rad, num_points)

#         # Punkte mit SEQUENZNUMMERN erstellen (0, 1, 2, ...)
#         points = []

#         for i in range(1, num_points):
#             # Sequenznummer = i
#             sequence = i - 1

#             # Winkel -> Steps
#             angles_deg = np.rad2deg(traj.q[i])
#             steps = []
#             for j in range(6):
#                 steps.append(
#                     self.robot.deg_2_steps(j, angles_deg[j], apply_home_offset=True)
#                 )

#             points.append((sequence, steps))

#         # Garantiere exaktes Ziel am Ende
#         if points:
#             points[-1] = (num_points - 1, target_steps.copy())

#         print(f"=== JOINT TRAJECTORY ===")
#         print(f"Points: {num_points} at {self.CONTROL_RATE_HZ}Hz")
#         print(f"Time: {move_time:.2f}s ({duration_ms}ms)")
#         print(f"Control period: {self.CONTROL_PERIOD_MS}ms")
#         print(f"Start steps: {start_steps}")
#         print(f"Target steps: {target_steps}")
#         print(f"First point (seq={points[0][0]}): {points[0][1]}")
#         print(f"Last point (seq={points[-1][0]}): {points[-1][1]}")
#         print("========================")

#         return points, duration_ms

#     def calculate_cartesian_trajectory(
#         self,
#         start_steps: List[int],
#         target_pose: np.ndarray,  # [x, y, z, rx, ry, rz] in mm/Grad
#         tcp_speed_mm_s: float = 50.0,
#     ) -> Tuple[List[Tuple[int, List[int]]], int]:
#         """Kartesische Trajektorie mit fester 100Hz Frequenz"""
#         # Aktuelle Winkel
#         start_rad = self.robot.steps_to_q_rad(start_steps, remove_home_offset=True)

#         # Aktuelle TCP-Pose
#         T_start: SE3 = self.robot.fkine(start_rad)

#         # Zielpose
#         R = rpy2r(target_pose[3], target_pose[4], target_pose[5], unit="deg")
#         p = np.array(
#             [
#                 target_pose[0] / 1000.0,
#                 target_pose[1] / 1000.0,
#                 target_pose[2] / 1000.0,
#             ]
#         )
#         T_target: SE3 = SE3.Rt(R, p)

#         # Distanz und Zeit berechnen
#         linear_distance = np.linalg.norm(T_target.t - T_start.t) * 1000  # m -> mm
#         move_time = linear_distance / tcp_speed_mm_s
#         move_time = max(0.5, min(move_time, 10.0))

#         # Anzahl Punkte basierend auf 100Hz
#         num_points = int(move_time * self.CONTROL_RATE_HZ)
#         num_points = max(10, min(num_points, 500))

#         # Synchronisierte Zeit berechnen
#         duration_ms = int(num_points * self.CONTROL_PERIOD_MS)
#         move_time = duration_ms / 1000.0

#         # Kartesische Trajektorie
#         traj_cart = cast(List[SE3], ctraj(T_start, T_target, num_points))

#         # IK für Ziel berechnen (für Fallback)
#         sol_target = self.robot.ikine_LM(T_target, q0=start_rad)
#         if sol_target.success:
#             target_rad = sol_target.q
#         else:
#             raise ValueError("IK für Zielpose fehlgeschlagen")

#         # Inverse Kinematik für jeden Punkt
#         points = []
#         last_angles = start_rad.copy()

#         for i in range(num_points):
#             # IK berechnen
#             sol = self.robot.ikine_LM(traj_cart[i], q0=last_angles)

#             if sol.success:
#                 joint_angles = sol.q
#                 last_angles = joint_angles

#                 # Winkel -> Steps
#                 angles_deg = np.rad2deg(joint_angles)
#                 steps = [
#                     self.robot.deg_2_steps(j, angles_deg[j], apply_home_offset=True)
#                     for j in range(6)
#                 ]
#                 points.append((i, steps))  # Sequenznummer = i
#             else:
#                 # Fallback: Lineare Interpolation im Joint-Space
#                 t = i / (num_points - 1) if num_points > 1 else 0
#                 joint_angles = start_rad + (target_rad - start_rad) * t
#                 angles_deg = np.rad2deg(joint_angles)
#                 steps = [
#                     self.robot.deg_2_steps(j, angles_deg[j], apply_home_offset=True)
#                     for j in range(6)
#                 ]
#                 points.append((i, steps))
#                 print(f"Warnung: IK bei Punkt {i} verwendet Fallback")

#         # Garantiere exaktes Ziel am Ende
#         if points:
#             target_steps_list = [
#                 self.robot.deg_2_steps(
#                     j, np.rad2deg(target_rad[j]), apply_home_offset=True
#                 )
#                 for j in range(6)
#             ]
#             points[-1] = (num_points - 1, target_steps_list)

#         print(f"=== CARTESIAN TRAJECTORY ===")
#         print(f"Points: {num_points} at {self.CONTROL_RATE_HZ}Hz")
#         print(f"Time: {move_time:.2f}s ({duration_ms}ms)")
#         print(f"Linear distance: {linear_distance:.1f}mm")
#         print(f"TCP speed: {tcp_speed_mm_s}mm/s")
#         print("=============================")

#         return points, duration_ms


# # !!!
import numpy as np
from roboticstoolbox import jtraj, ctraj
from math import pi
from typing import cast, List, Tuple
import struct
from spatialmath import SE3
from spatialmath.base import rpy2r
from app.robotics.robot import Robot


class TrajectoryCalculator:
    """Nutzt Ihre bestehende Robot-Klasse für Trajektorien"""

    def __init__(self, robot: Robot):
        self.robot = robot

    def calculate_joint_trajectory(
        self,
        start_steps: List[int],
        target_steps: List[int],
        move_time: float = 5.0,
        control_rate_hz: int = 100,
    ) -> List[Tuple[int, List[int]]]:
        """
        Berechnet Joint-Space Trajektorie

        WICHTIG: Der erste Punkt (Index 0) wird NICHT gesendet!
        Der Teensy startet bereits an start_steps, der erste gesendete Punkt
        ist die Position nach dem ersten Zeitschritt.
        """
        # 1. Minimal/Maximal Punkte basierend auf Move-Zeit
        if move_time <= 1.0:
            # Kurze Moves: viele Punkte für Smoothness
            points_per_second = 200
        elif move_time <= 5.0:
            # Mittlere Moves: 100 Hz
            points_per_second = 100
        else:
            # Lange Moves: 50 Hz (spart Speicher)
            points_per_second = 50

        # Anzahl Punkte
        num_points = int(move_time * points_per_second)
        # num_points = int(move_time * control_rate_hz)

        num_points = max(10, min(num_points, 1000))

        # Steps -> Radians
        # start_rad = self.robot.steps_to_q_rad(start_steps)
        # target_rad = self.robot.steps_to_q_rad(target_steps)
        start_rad = self.robot.steps_to_q_rad(start_steps, remove_home_offset=True)
        target_rad = self.robot.steps_to_q_rad(target_steps, remove_home_offset=True)

        # Trajektorie berechnen (jtraj gibt num_points Punkte zurück)
        traj = jtraj(start_rad, target_rad, num_points)

        # Punkte für Teensy erstellen
        # WICHTIG: Wir überspringen den ersten Punkt (Index 0),
        # da der Teensy bereits an der Startposition ist!
        points = []

        for i in range(1, num_points):  # Start bei 1, nicht 0!
            time_ms = int(i * (move_time * 1000 / num_points))

            # Winkel -> Steps
            angles_deg = np.rad2deg(traj.q[i])
            steps = []
            for j in range(6):
                steps.append(
                    self.robot.deg_2_steps(j, angles_deg[j], apply_home_offset=True)
                )

            points.append((time_ms, steps))

        # Optional: Letzten Punkt explizit auf Ziel setzen (für Genauigkeit)
        if points:
            final_time = int(move_time * 1000)
            points[-1] = (final_time, target_steps.copy())

        print(f"Trajectory calculated: {len(points)} points over {move_time}s")
        print(f"Start: {start_steps}")
        if points:
            print(f"First point (t={points[0][0]}ms): {points[0][1]}")
            print(f"Last point (t={points[-1][0]}ms): {points[-1][1]}")
        print(f"Target: {target_steps}")

        return points

    def calculate_cartesian_trajectory(
        self,
        start_steps: List[int],
        target_pose: np.ndarray,  # [x, y, z, rx, ry, rz] in mm/Grad
        tcp_speed_mm_s: float = 50.0,
        control_rate_hz: int = 100,
    ) -> List[Tuple[int, List[int]]]:
        """Berechnet lineare kartesische Trajektorie"""
        # 1. Aktuelle Winkel
        start_rad = self.robot.steps_to_q_rad(start_steps)

        # 2. Aktuelle TCP-Pose berechnen
        T_start: SE3 = self.robot.fkine(start_rad)

        # 3. Zielpose erstellen
        R = rpy2r(target_pose[3], target_pose[4], target_pose[5], unit="deg")
        p = np.array(
            [
                target_pose[0] / 1000.0,
                target_pose[1] / 1000.0,
                target_pose[2] / 1000.0,
            ]
        )
        T_target: SE3 = SE3.Rt(R, p)

        # 4. Distanz und Zeit berechnen
        linear_distance = np.linalg.norm(T_target.t - T_start.t) * 1000  # m -> mm
        move_time = linear_distance / tcp_speed_mm_s
        move_time = max(0.5, min(move_time, 10.0))  # 0.5-10 Sekunden

        # 5. Kartesische Trajektorie
        num_points = int(move_time * control_rate_hz)
        num_points = max(10, min(num_points, 500))

        traj_cart = cast(List[SE3], ctraj(T_start, T_target, num_points))

        # 6. Inverse Kinematik für jeden Punkt (auch hier: skip ersten Punkt)
        points = []
        last_angles = start_rad.copy()

        for i in range(1, num_points):  # Skip first point!
            # IK berechnen
            sol = self.robot.ikine_LM(traj_cart[i], q0=last_angles)

            if sol.success:
                joint_angles = sol.q
                last_angles = joint_angles

                time_ms = int(i * (move_time * 1000 / num_points))

                # Winkel -> Steps
                angles_deg = np.rad2deg(joint_angles)
                steps = [self.robot.deg_2_steps(j, angles_deg[j]) for j in range(6)]

                points.append((time_ms, steps))
            else:
                print(f"Warning: IK failed at point {i}")

        return points


# !!!
# import numpy as np
# from roboticstoolbox import jtraj, ctraj
# from math import pi
# from typing import cast, List, Tuple
# import struct
# from spatialmath import SE3
# from spatialmath.base import rpy2r
# from app.robotics.robot import Robot


# class TrajectoryCalculator:
#     """Nutzt Ihre bestehende Robot-Klasse für Trajektorien"""

#     def __init__(self, robot: Robot):
#         self.robot = robot
#         # self.constants = self.robot.constants

#     def calculate_joint_trajectory(
#         self,
#         start_steps: List[int],
#         target_steps: List[int],
#         move_time: float = 2.0,
#         control_rate_hz: int = 100,
#     ) -> List[Tuple[int, List[int]]]:
#         """Berechnet Joint-Space Trajektorie"""
#         # Steps -> Radians
#         start_rad = self.robot.steps_to_q_rad(start_steps)
#         target_rad = self.robot.steps_to_q_rad(target_steps)

#         # Anzahl Punkte
#         num_points = int(move_time * control_rate_hz)
#         num_points = max(10, min(num_points, 500))

#         # Trajektorie berechnen
#         traj = jtraj(start_rad, target_rad, num_points)

#         # Punkte für Teensy erstellen
#         points = []
#         for i in range(num_points):
#             time_ms = int(i * (move_time * 1000 / num_points))

#             # Winkel -> Steps (über Ihre bestehende Methode)
#             angles_deg = np.rad2deg(traj.q[i])
#             steps = []
#             for j in range(6):
#                 steps.append(self.robot.deg_2_steps(j, angles_deg[j]))

#             points.append((time_ms, steps))

#         return points

#     def calculate_cartesian_trajectory(
#         self,
#         start_steps: List[int],
#         target_pose: np.ndarray,  # [x, y, z, rx, ry, rz] in mm/Grad
#         tcp_speed_mm_s: float = 50.0,
#         control_rate_hz: int = 100,
#     ) -> List[Tuple[int, List[int]]]:
#         """Berechnet lineare kartesische Trajektorie"""
#         # 1. Aktuelle Winkel
#         start_rad = self.robot.steps_to_q_rad(start_steps)

#         # 2. Aktuelle TCP-Pose berechnen
#         T_start: SE3 = self.robot.fkine(start_rad)

#         # # 3. Zielpose erstellen
#         R = rpy2r(target_pose[3], target_pose[4], target_pose[5], unit="deg")
#         p = np.array(
#             [
#                 target_pose[0] / 1000.0,
#                 target_pose[1] / 1000.0,
#                 target_pose[2] / 1000.0,
#             ]
#         )

#         T_target: SE3 = SE3.Rt(R, p)

#         # 4. Distanz und Zeit berechnen
#         linear_distance = np.linalg.norm(T_target.t - T_start.t) * 1000  # m -> mm
#         move_time = linear_distance / tcp_speed_mm_s
#         move_time = max(0.5, min(move_time, 10.0))  # 0.5-10 Sekunden

#         # 5. Kartesische Trajektorie
#         num_points = int(move_time * control_rate_hz)
#         num_points = max(10, min(num_points, 500))

#         traj_cart = cast(List[SE3], ctraj(T_start, T_target, num_points))

#         # 6. Inverse Kinematik für jeden Punkt
#         points = []
#         last_angles = start_rad.copy()

#         for i in range(num_points):
#             # IK berechnen
#             sol = self.robot.ikine_LM(traj_cart[i], q0=last_angles)

#             if sol.success:
#                 joint_angles = sol.q
#                 last_angles = joint_angles

#                 time_ms = int(i * (move_time * 1000 / num_points))

#                 # Winkel -> Steps
#                 angles_deg = np.rad2deg(joint_angles)
#                 steps = [self.robot.deg_2_steps(j, angles_deg[j]) for j in range(6)]

#                 points.append((time_ms, steps))
#             else:
#                 print(f"Warning: IK failed at point {i}")
#                 # Fallback: Joint-Space Interpolation
#                 if i > 0 and i < num_points - 1:
#                     continue

#         return points
