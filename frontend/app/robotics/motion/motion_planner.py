from typing import Literal, Sequence, List
import numpy as np
import numpy.typing as npt
from spatialmath import SE3
import struct

from app.robotics.robot import Robot
from app.constants.ms6_r_constants import MS6_R_CONSTANTS
from app.core.shared.shared_data import shared_data
from app.core.packet_builder import packet_builder
from app.constants.com_protocol import (
    CMD_MOVE_TO_POS,
    INFO_REACHED,
    CMD_TRAJ_START,
    CMD_TRAJ_DATA,
    CMD_TRAJ_EXEC,
    CMD_TRAJ_END,
    INFO_TRAJ_START_ACK,
    INFO_TRAJ_CHUNK_ACK,
    INFO_TRAJ_EXEC_ACK,
    INFO_TRAJ_UPLOAD_COMPLETE,
)
from app.robotics.motion.trajectory_calculator import TrajectoryCalculator


class EnhancedMotionPlanner:
    def __init__(self):
        self.robot = Robot(constants=MS6_R_CONSTANTS)
        self.traj_calc = TrajectoryCalculator(self.robot)

        # Zustände für Trajektorien-Senden
        self._traj_state = "IDLE"
        self._traj_points = []
        self._current_chunk = 0
        self._chunk_size = 5

        # Timeout-Handling
        self._last_ack_time = 0
        self._ack_timeout_ms = 2000

        # Sequenz-Status
        self._sequence = []
        self._seq_index = 0
        self._seq_running = False
        self._seq_move_time = 2.0

    def log(self, data):
        print("Log: ", data)

    def start_trajectory_sequence(
        self, sequence: List[List[int]], move_time: float = 2.0
    ):
        """Startet eine Sequenz von Positionen als Trajektorien"""
        if not sequence:
            print("Empty sequence")
            return

        self._sequence = sequence
        self._seq_index = 0
        self._seq_running = True
        self._seq_move_time = move_time

        print(f"Starting trajectory sequence with {len(sequence)} positions")
        self._execute_next_in_sequence()

    def _execute_next_in_sequence(self):
        """Berechnet und sendet Trajektorie zum nächsten Punkt in der Sequenz"""
        if not self._seq_running:
            return

        if self._seq_index >= len(self._sequence):
            print("Trajectory sequence complete!")
            self._seq_running = False
            return

        # Aktuelle Position holen
        current_steps = shared_data.get_steps()
        if current_steps is None:
            print("Error: No current position")
            self._seq_running = False
            return

        # Nächste Zielposition
        target_steps = self._sequence[self._seq_index]

        print(
            f"\n=== Moving to position {self._seq_index + 1}/{len(self._sequence)} ==="
        )
        print(f"Target: {target_steps}")

        # Optional: Prüfen ob bereits am Ziel (Tolerance: 10 steps)
        if all(abs(current_steps[i] - target_steps[i]) < 10 for i in range(6)):
            print("Already at target position, skipping...")
            self._seq_index += 1
            self._execute_next_in_sequence()
            return

        # Trajektorie berechnen und senden
        self.plan_and_send_trajectory(
            target_steps=target_steps, move_type="joint", move_time=self._seq_move_time
        )

        self._seq_index += 1

    def plan_and_send_trajectory(
        self,
        target_steps: List[int],
        move_type: str = "joint",
        move_time: float = 2.0,
        tcp_speed: float = 50.0,
        target_pose: np.ndarray | None = None,
    ):
        """Berechnet und sendet Trajektorie"""
        if self._traj_state != "IDLE":
            print("Error: Trajectory already in progress")
            return

        current_steps = shared_data.get_steps()
        if current_steps is None:
            print("Error: No current position")
            return

        print(f"\nCalculating {move_type} trajectory...")
        print(f"From: {current_steps}")
        print(f"To:   {target_steps}")
        print(f"Time: {move_time}s")

        # Trajektorie berechnen
        if move_type == "joint":
            self._traj_points = self.traj_calc.calculate_joint_trajectory(
                current_steps, target_steps, move_time
            )
        elif move_type == "cartesian" and target_pose is not None:
            self._traj_points = self.traj_calc.calculate_cartesian_trajectory(
                current_steps, target_pose, tcp_speed
            )
        else:
            print("Error: Invalid parameters")
            return

        if not self._traj_points:
            print("Error: No trajectory points calculated")
            return

        print(f"Trajectory calculated: {len(self._traj_points)} points")
        self._start_trajectory_send()

    def _start_trajectory_send(self):
        """Startet den Sende-Prozess"""
        if not self._traj_points:
            return

        total_time_ms = self._traj_points[-1][0]

        # CMD_TRAJ_START senden (ZEITSTEMPEL!)
        start_data = struct.pack("<II", len(self._traj_points), total_time_ms)
        packet = packet_builder.build_packet(CMD_TRAJ_START, start_data)

        shared_data.set_data_out(packet)
        self._traj_state = "WAIT_START_ACK"
        self._last_ack_time = self._get_current_time()
        print(
            f"Sent: CMD_TRAJ_START ({len(self._traj_points)} points, {total_time_ms}ms)"
        )

    def on_data_in(self, value: int):
        """Verarbeitet eingehende Daten (ACKs)"""
        if self._traj_state == "WAIT_START_ACK":
            if value == INFO_TRAJ_START_ACK:
                print("Received: INFO_TRAJ_START_ACK")
                self._current_chunk = 0
                self._send_next_chunk()

        elif self._traj_state == "WAIT_CHUNK_ACK":
            if value == INFO_TRAJ_CHUNK_ACK:
                self._send_next_chunk()

        elif self._traj_state == "WAIT_END_ACK":
            if value == INFO_TRAJ_UPLOAD_COMPLETE:
                print("Received: INFO_TRAJ_UPLOAD_COMPLETE")
                packet = packet_builder.build_packet(CMD_TRAJ_EXEC, b"")
                shared_data.set_data_out(packet)
                self._traj_state = "WAIT_EXEC_ACK"
                self._last_ack_time = self._get_current_time()
                print("Sent: CMD_TRAJ_EXEC")

        elif self._traj_state == "WAIT_EXEC_ACK":
            if value == INFO_TRAJ_EXEC_ACK:
                print("Received: TRAJ_EXEC_ACK - Trajectory execution started")
                self._traj_state = "EXECUTING"

        # INFO_REACHED vom Teensy
        if value == INFO_REACHED:
            print("Received: INFO_REACHED - Trajectory complete\n")

            if self._traj_state == "EXECUTING":
                self._traj_state = "IDLE"
                self._traj_points = []
                self._current_chunk = 0

                # Wenn Sequenz läuft, nächsten Punkt anfahren
                if self._seq_running:
                    print("Continuing to next position in sequence...")
                    self._execute_next_in_sequence()

    def _send_next_chunk(self):
        """Sendet den nächsten Chunk - MIT ZEITSTEMPELN!"""
        if not self._traj_points:
            return

        start_idx = self._current_chunk * self._chunk_size
        end_idx = min(start_idx + self._chunk_size, len(self._traj_points))

        if start_idx >= len(self._traj_points):
            # Alle Chunks gesendet -> CMD_TRAJ_END
            packet = packet_builder.build_packet(CMD_TRAJ_END, b"")
            shared_data.set_data_out(packet)
            self._traj_state = "WAIT_END_ACK"
            self._last_ack_time = self._get_current_time()
            print("Sent: CMD_TRAJ_END")
            return

        # Chunk-Daten bauen (ZEITSTEMPEL in Millisekunden!)
        chunk_data = bytearray()
        chunk_data.append(self._current_chunk)

        for i in range(start_idx, end_idx):
            time_ms, steps = self._traj_points[i]  # ZEITSTEMPEL!
            # Format: uint32 time_ms + 6x int32 steps
            chunk_data.extend(struct.pack("<I6i", time_ms, *steps))

        # Paket senden
        packet = packet_builder.build_packet(CMD_TRAJ_DATA, chunk_data)
        shared_data.set_data_out(packet)
        self._traj_state = "WAIT_CHUNK_ACK"
        self._last_ack_time = self._get_current_time()

        print(
            f"Sent: CMD_TRAJ_DATA chunk {self._current_chunk} "
            f"({end_idx - start_idx} points, t={self._traj_points[start_idx][0]}-{self._traj_points[end_idx-1][0]}ms)"
        )
        self._current_chunk += 1

    def update(self):
        """Wird regelmäßig aufgerufen"""
        current_time = self._get_current_time()

        # Timeout für Trajektorien-ACKs
        if self._traj_state in (
            "WAIT_START_ACK",
            "WAIT_CHUNK_ACK",
            "WAIT_END_ACK",
            "WAIT_EXEC_ACK",
        ):
            if current_time - self._last_ack_time > self._ack_timeout_ms:
                print(f"Error: Trajectory ACK timeout in state {self._traj_state}")
                self._traj_state = "IDLE"
                self._traj_points = []
                self._current_chunk = 0
                self._seq_running = False

    def _get_current_time(self):
        import time

        return int(time.time() * 1000)


#!!!!
# from typing import Literal, Sequence, List
# import numpy as np
# import numpy.typing as npt
# from spatialmath import SE3
# import struct

# from app.robotics.robot import Robot
# from app.constants.ms6_r_constants import MS6_R_CONSTANTS
# from app.core.shared.shared_data import shared_data
# from app.core.packet_builder import packet_builder
# from app.constants.com_protocol import (
#     CMD_MOVE_TO_POS,
#     INFO_REACHED,
#     CMD_TRAJ_START,
#     CMD_TRAJ_DATA,
#     CMD_TRAJ_EXEC,
#     CMD_TRAJ_END,
#     INFO_TRAJ_START_ACK,
#     INFO_TRAJ_CHUNK_ACK,
#     INFO_TRAJ_EXEC_ACK,
#     INFO_TRAJ_UPLOAD_COMPLETE,
# )
# from app.robotics.motion.trajectory_calculator import TrajectoryCalculator


# class EnhancedMotionPlanner:
#     def __init__(self):
#         self.robot = Robot(constants=MS6_R_CONSTANTS)
#         self.traj_calc = TrajectoryCalculator(self.robot)

#         # Zustände für Trajektorien-Senden
#         self._traj_state = "IDLE"
#         self._traj_points = []
#         self._current_chunk = 0
#         self._chunk_size = 5

#         # Timeout-Handling
#         self._last_ack_time = 0
#         self._ack_timeout_ms = 2000

#         # Sequenz-Status
#         self._sequence = []
#         self._seq_index = 0
#         self._seq_running = False
#         self._seq_move_time = 2.0  # Standard-Bewegungszeit

#     def log(self, data):
#         print("Log: ", data)

#     def start_trajectory_sequence(
#         self, sequence: List[List[int]], move_time: float = 2.0
#     ):
#         """Startet eine Sequenz von Positionen als Trajektorien"""
#         if not sequence:
#             print("Empty sequence")
#             return

#         self._sequence = sequence
#         self._seq_index = 0
#         self._seq_running = True
#         self._seq_move_time = move_time

#         print(f"Starting trajectory sequence with {len(sequence)} positions")
#         self._execute_next_in_sequence()

#     def _execute_next_in_sequence(self):
#         """Berechnet und sendet Trajektorie zum nächsten Punkt in der Sequenz"""
#         if not self._seq_running:
#             return

#         if self._seq_index >= len(self._sequence):
#             print("Trajectory sequence complete!")
#             self._seq_running = False
#             return

#         # Aktuelle Position holen
#         current_steps = shared_data.get_steps()
#         if current_steps is None:
#             print("Error: No current position")
#             self._seq_running = False
#             return

#         # Nächste Zielposition
#         target_steps = self._sequence[self._seq_index]

#         print(
#             f"Moving to position {self._seq_index + 1}/{len(self._sequence)}: {target_steps}"
#         )

#         # Optional: Prüfen ob bereits am Ziel (Tolerance: 10 steps)
#         if all(abs(current_steps[i] - target_steps[i]) < 10 for i in range(6)):
#             print("Already at target position, skipping...")
#             self._seq_index += 1
#             # Sofort nächsten Punkt anfahren
#             self._execute_next_in_sequence()
#             return

#         # Trajektorie berechnen und senden
#         self.plan_and_send_trajectory(
#             target_steps=target_steps, move_type="joint", move_time=self._seq_move_time
#         )

#         self._seq_index += 1

#     def plan_and_send_trajectory(
#         self,
#         target_steps: List[int],
#         move_type: str = "joint",
#         move_time: float = 2.0,
#         tcp_speed: float = 50.0,
#         target_pose: np.ndarray | None = None,
#     ):
#         """Berechnet und sendet Trajektorie"""
#         if self._traj_state != "IDLE":
#             print("Error: Trajectory already in progress")
#             return

#         current_steps = shared_data.get_steps()
#         if current_steps is None:
#             print("Error: No current position")
#             return

#         print(f"Calculating {move_type} trajectory...")
#         print(f"From: {current_steps}")
#         print(f"To: {target_steps}")

#         # Trajektorie berechnen
#         if move_type == "joint":
#             self._traj_points = self.traj_calc.calculate_joint_trajectory(
#                 current_steps, target_steps, move_time
#             )
#         elif move_type == "cartesian" and target_pose is not None:
#             self._traj_points = self.traj_calc.calculate_cartesian_trajectory(
#                 current_steps, target_pose, tcp_speed
#             )
#         else:
#             print("Error: Invalid parameters")
#             return

#         if not self._traj_points:
#             print("Error: No trajectory points calculated")
#             return

#         print(f"Trajectory calculated: {len(self._traj_points)} points")
#         self._start_trajectory_send()

#     def _start_trajectory_send(self):
#         """Startet den Sende-Prozess"""
#         if not self._traj_points:
#             return

#         total_time_ms = self._traj_points[-1][0]

#         # CMD_TRAJ_START senden
#         start_data = struct.pack("<II", len(self._traj_points), total_time_ms)
#         packet = packet_builder.build_packet(CMD_TRAJ_START, start_data)

#         shared_data.set_data_out(packet)
#         self._traj_state = "WAIT_START_ACK"
#         self._last_ack_time = self._get_current_time()
#         print(
#             f"Sent: CMD_TRAJ_START ({len(self._traj_points)} points, {total_time_ms}ms)"
#         )

#     def on_data_in(self, value: int):
#         """Verarbeitet eingehende Daten (ACKs)"""
#         # Trajektorien-ACKs
#         if self._traj_state == "WAIT_START_ACK":
#             if value == INFO_TRAJ_START_ACK:
#                 print("Received: INFO_TRAJ_START_ACK")
#                 self._current_chunk = 0
#                 self._send_next_chunk()

#         elif self._traj_state == "WAIT_CHUNK_ACK":
#             if value == INFO_TRAJ_CHUNK_ACK:
#                 # print("Received: INFO_TRAJ_CHUNK_ACK")
#                 self._send_next_chunk()

#         elif self._traj_state == "WAIT_END_ACK":
#             if value == INFO_TRAJ_UPLOAD_COMPLETE:
#                 print("Received: INFO_TRAJ_UPLOAD_COMPLETE")
#                 # Jetzt CMD_TRAJ_EXEC senden
#                 packet = packet_builder.build_packet(CMD_TRAJ_EXEC, b"")
#                 shared_data.set_data_out(packet)
#                 self._traj_state = "WAIT_EXEC_ACK"
#                 self._last_ack_time = self._get_current_time()
#                 print("Sent: CMD_TRAJ_EXEC")

#         elif self._traj_state == "WAIT_EXEC_ACK":
#             if value == INFO_TRAJ_EXEC_ACK:
#                 print("Received: TRAJ_EXEC_ACK - Trajectory execution started")
#                 self._traj_state = "EXECUTING"  # Trajektorie läuft jetzt

#         # WICHTIG: INFO_REACHED vom Teensy (am Ende der Trajektorie)
#         if value == INFO_REACHED:
#             print("Received: INFO_REACHED - Trajectory complete")

#             if self._traj_state == "EXECUTING":
#                 self._traj_state = "IDLE"
#                 self._traj_points = []
#                 self._current_chunk = 0

#                 # Wenn Sequenz läuft, nächsten Punkt anfahren
#                 if self._seq_running:
#                     print("Continuing to next position in sequence...")
#                     self._execute_next_in_sequence()

#     def _send_next_chunk(self):
#         """Sendet den nächsten Chunk"""
#         if not self._traj_points:
#             return

#         start_idx = self._current_chunk * self._chunk_size
#         end_idx = min(start_idx + self._chunk_size, len(self._traj_points))

#         if start_idx >= len(self._traj_points):
#             # Alle Chunks gesendet -> CMD_TRAJ_END senden
#             packet = packet_builder.build_packet(CMD_TRAJ_END, b"")
#             shared_data.set_data_out(packet)
#             self._traj_state = "WAIT_END_ACK"
#             self._last_ack_time = self._get_current_time()
#             print("Sent: CMD_TRAJ_END")
#             return

#         # Chunk-Daten bauen
#         chunk_data = bytearray()
#         chunk_data.append(self._current_chunk)

#         for i in range(start_idx, end_idx):
#             time_ms, steps = self._traj_points[i]
#             chunk_data.extend(struct.pack("<I6i", time_ms, *steps))

#         # Paket senden
#         packet = packet_builder.build_packet(CMD_TRAJ_DATA, chunk_data)
#         shared_data.set_data_out(packet)
#         self._traj_state = "WAIT_CHUNK_ACK"
#         self._last_ack_time = self._get_current_time()

#         print(
#             f"Sent: CMD_TRAJ_DATA chunk {self._current_chunk} ({end_idx - start_idx} points)"
#         )
#         self._current_chunk += 1

#     def update(self):
#         """Wird regelmäßig aufgerufen"""
#         current_time = self._get_current_time()

#         # Timeout für Trajektorien-ACKs
#         if self._traj_state in (
#             "WAIT_START_ACK",
#             "WAIT_CHUNK_ACK",
#             "WAIT_END_ACK",
#             "WAIT_EXEC_ACK",
#         ):
#             if current_time - self._last_ack_time > self._ack_timeout_ms:
#                 print(f"Error: Trajectory ACK timeout in state {self._traj_state}")
#                 self._traj_state = "IDLE"
#                 self._traj_points = []
#                 self._current_chunk = 0
#                 self._seq_running = False  # Sequenz auch stoppen

#     def _get_current_time(self):
#         import time

#         return int(time.time() * 1000)


#!!!!
# from typing import Literal, Sequence, List
# import numpy as np
# import numpy.typing as npt
# from spatialmath import SE3
# import struct

# from app.robotics.robot import Robot
# from app.constants.ms6_r_constants import MS6_R_CONSTANTS
# from app.core.shared.shared_data import shared_data
# from app.core.packet_builder import packet_builder
# from app.constants.com_protocol import (
#     CMD_MOVE_TO_POS,
#     INFO_REACHED,
#     CMD_TRAJ_START,
#     CMD_TRAJ_DATA,
#     CMD_TRAJ_EXEC,
#     CMD_TRAJ_END,
#     INFO_TRAJ_START_ACK,
#     INFO_TRAJ_CHUNK_ACK,
#     INFO_TRAJ_EXEC_ACK,
#     INFO_TRAJ_UPLOAD_COMPLETE,
# )
# from app.robotics.motion.trajectory_calculator import TrajectoryCalculator


# class EnhancedMotionPlanner:
#     def __init__(self):
#         self.robot = Robot(constants=MS6_R_CONSTANTS)
#         self.traj_calc = TrajectoryCalculator(self.robot)

#         # Zustände für Trajektorien-Senden
#         self._traj_state = "IDLE"
#         self._traj_points = []
#         self._current_chunk = 0
#         self._chunk_size = 5

#         # Timeout-Handling
#         self._last_ack_time = 0
#         self._ack_timeout_ms = 2000

#         # Sequenz-Status
#         self._sequence = []
#         self._seq_index = 0
#         self._seq_running = False
#         self._seq_move_time = 2.0  # Standard-Bewegungszeit

#     def log(self, data):
#         print("Log: ", data)

#     def start_trajectory_sequence(
#         self, sequence: List[List[int]], move_time: float = 2.0
#     ):
#         """Startet eine Sequenz von Positionen als Trajektorien"""
#         if not sequence:
#             print("Empty sequence")
#             return

#         self._sequence = sequence
#         self._seq_index = 0
#         self._seq_running = True
#         self._seq_move_time = move_time

#         print(f"Starting trajectory sequence with {len(sequence)} positions")
#         self._execute_next_in_sequence()

#     def _execute_next_in_sequence(self):
#         """Berechnet und sendet Trajektorie zum nächsten Punkt in der Sequenz"""
#         if not self._seq_running:
#             return

#         if self._seq_index >= len(self._sequence):
#             print("Trajectory sequence complete!")
#             self._seq_running = False
#             return

#         # Aktuelle Position holen
#         current_steps = shared_data.get_steps()
#         if current_steps is None:
#             print("Error: No current position")
#             self._seq_running = False
#             return

#         # Nächste Zielposition
#         target_steps = self._sequence[self._seq_index]

#         print(
#             f"Moving to position {self._seq_index + 1}/{len(self._sequence)}: {target_steps}"
#         )

#         # Trajektorie berechnen und senden
#         self.plan_and_send_trajectory(
#             target_steps=target_steps, move_type="joint", move_time=self._seq_move_time
#         )

#         self._seq_index += 1

#     def plan_and_send_trajectory(
#         self,
#         target_steps: List[int],
#         move_type: str = "joint",
#         move_time: float = 2.0,
#         tcp_speed: float = 50.0,
#         target_pose: np.ndarray | None = None,
#     ):
#         """Berechnet und sendet Trajektorie"""
#         if self._traj_state != "IDLE":
#             print("Error: Trajectory already in progress")
#             return

#         current_steps = shared_data.get_steps()
#         if current_steps is None:
#             print("Error: No current position")
#             return

#         print(f"Calculating {move_type} trajectory...")

#         # Trajektorie berechnen
#         if move_type == "joint":
#             self._traj_points = self.traj_calc.calculate_joint_trajectory(
#                 current_steps, target_steps, move_time
#             )
#         elif move_type == "cartesian" and target_pose is not None:
#             self._traj_points = self.traj_calc.calculate_cartesian_trajectory(
#                 current_steps, target_pose, tcp_speed
#             )
#         else:
#             print("Error: Invalid parameters")
#             return

#         if not self._traj_points:
#             print("Error: No trajectory points calculated")
#             return

#         print(f"Trajectory calculated: {len(self._traj_points)} points")
#         self._start_trajectory_send()

#     def _start_trajectory_send(self):
#         """Startet den Sende-Prozess"""
#         if not self._traj_points:
#             return

#         total_time_ms = self._traj_points[-1][0]

#         # CMD_TRAJ_START senden
#         start_data = struct.pack("<II", len(self._traj_points), total_time_ms)
#         packet = packet_builder.build_packet(CMD_TRAJ_START, start_data)

#         shared_data.set_data_out(packet)
#         self._traj_state = "WAIT_START_ACK"
#         self._last_ack_time = self._get_current_time()
#         print("Sent: CMD_TRAJ_START")

#     def on_data_in(self, value: int):
#         """Verarbeitet eingehende Daten (ACKs)"""
#         # Trajektorien-ACKs
#         if self._traj_state == "WAIT_START_ACK":
#             if value == INFO_TRAJ_START_ACK:
#                 print("Received: INFO_TRAJ_START_ACK")
#                 self._current_chunk = 0
#                 self._send_next_chunk()

#         elif self._traj_state == "WAIT_CHUNK_ACK":
#             if value == INFO_TRAJ_CHUNK_ACK:
#                 # print("Received: INFO_TRAJ_CHUNK_ACK")
#                 self._send_next_chunk()

#         elif self._traj_state == "WAIT_END_ACK":
#             if value == INFO_TRAJ_UPLOAD_COMPLETE:
#                 print("Received: INFO_TRAJ_UPLOAD_COMPLETE")
#                 # Jetzt CMD_TRAJ_EXEC senden
#                 packet = packet_builder.build_packet(CMD_TRAJ_EXEC, b"")
#                 shared_data.set_data_out(packet)
#                 self._traj_state = "WAIT_EXEC_ACK"
#                 self._last_ack_time = self._get_current_time()
#                 print("Sent: CMD_TRAJ_EXEC")

#         elif self._traj_state == "WAIT_EXEC_ACK":
#             if value == INFO_TRAJ_EXEC_ACK:
#                 print("Received: TRAJ_EXEC_ACK - Trajectory execution started")
#                 self._traj_state = "EXECUTING"  # Trajektorie läuft jetzt

#         # WICHTIG: INFO_REACHED vom Teensy (am Ende der Trajektorie)
#         elif value == INFO_REACHED:
#             print("Received: INFO_REACHED - Trajectory complete")

#             if self._traj_state == "EXECUTING":
#                 self._traj_state = "IDLE"
#                 self._traj_points = []
#                 self._current_chunk = 0

#                 # Wenn Sequenz läuft, nächsten Punkt anfahren
#                 if self._seq_running:
#                     self._execute_next_in_sequence()

#     def _send_next_chunk(self):
#         """Sendet den nächsten Chunk"""
#         if not self._traj_points:
#             return

#         start_idx = self._current_chunk * self._chunk_size
#         end_idx = min(start_idx + self._chunk_size, len(self._traj_points))

#         if start_idx >= len(self._traj_points):
#             # Alle Chunks gesendet -> CMD_TRAJ_END senden
#             packet = packet_builder.build_packet(CMD_TRAJ_END, b"")
#             shared_data.set_data_out(packet)
#             self._traj_state = "WAIT_END_ACK"
#             self._last_ack_time = self._get_current_time()
#             print("Sent: CMD_TRAJ_END")
#             return

#         # Chunk-Daten bauen
#         chunk_data = bytearray()
#         chunk_data.append(self._current_chunk)

#         for i in range(start_idx, end_idx):
#             time_ms, steps = self._traj_points[i]
#             chunk_data.extend(struct.pack("<I6i", time_ms, *steps))

#         # Paket senden
#         packet = packet_builder.build_packet(CMD_TRAJ_DATA, chunk_data)
#         shared_data.set_data_out(packet)
#         self._traj_state = "WAIT_CHUNK_ACK"
#         self._last_ack_time = self._get_current_time()

#         # print(f"Sent: CMD_TRAJ_DATA chunk {self._current_chunk}")
#         self._current_chunk += 1

#     def update(self):
#         """Wird regelmäßig aufgerufen"""
#         current_time = self._get_current_time()

#         # Timeout für Trajektorien-ACKs
#         # if self._traj_state != "IDLE":
#         if self._traj_state in (
#             "WAIT_START_ACK",
#             "WAIT_CHUNK_ACK",
#             "WAIT_END_ACK",
#             "WAIT_EXEC_ACK",
#         ):
#             if current_time - self._last_ack_time > self._ack_timeout_ms:
#                 print(f"Error: Trajectory ACK timeout in state {self._traj_state}")
#                 self._traj_state = "IDLE"
#                 self._traj_points = []
#                 self._current_chunk = 0
#                 self._seq_running = False  # Sequenz auch stoppen

#     def _get_current_time(self):
#         import time

#         return int(time.time() * 1000)


# class MotionPlanner:
#     def __init__(self):
#         # self.R = Robot(constants=MS6_R_CONSTANTS)
#         # self._dq_accum = np.zeros(6)

#         # # Motor direction mapping (+1: +Steps = CCW, -1: +Steps = CW)
#         # self.motor_sign = np.array([-1, -1, -1, -1, +1, -1], dtype=int)

#         # # Jog parameters - diese kannst du anpassen
#         # self.min_step_size = 20  # Minimum Steps pro Ausgabe
#         # self.target_step_size = 40  # Ziel Steps pro Ausgabe
#         # self.max_step_size = 80  # Maximum Steps pro Ausgabe
#         self._sequence = []
#         self._index = 0
#         self._running = False

#     def log(self, data):
#         print("Log: ", data)
#         # pass

#     def start_sequence(self, seq: list[list[int]]):
#         self._sequence = seq
#         self._index = 0
#         self._running = True
#         self._send_next_point()

#     def _send_next_point(self):
#         if not self._running:
#             return

#         if self._index >= len(self._sequence):
#             print("Sequence complete")
#             self._running = False
#             return

#         pos = self._sequence[self._index]
#         self._index += 1

#         packet = packet_builder.build_packet(CMD_MOVE_TO_POS, pos)
#         shared_data.set_data_out(packet)

#     def on_data_in(self, value: int):
#         if value == INFO_REACHED and self._running:
#             self._send_next_point()


# class EnhancedMotionPlanner(MotionPlanner):
#     def __init__(self):
#         super().__init__()
#         self.robot = Robot(constants=MS6_R_CONSTANTS)
#         self.traj_calc = TrajectoryCalculator(self.robot)

#         # Zustände für Trajektorien-Senden
#         self._traj_state = "IDLE"
#         self._traj_points = []
#         self._current_chunk = 0
#         self._chunk_size = 5  # 5 Punkte = 140 Bytes

#         # Timeout-Handling
#         self._last_ack_time = 0
#         self._ack_timeout_ms = 2000  # 2 Sekunden

#         # # Für Rückwärtskompatibilität
#         # self._sequence = []
#         # self._seq_index = 0
#         # self._seq_running = False

#         # Teach-Positionen mit Parametern
#         self._teach_sequence = (
#             []
#         )  # Liste von Dicts: {"steps": [...], "move_time": 2.0, "type": "joint"}
#         self._teach_index = 0
#         self._running_teach_sequence = False

#     def plan_and_send_trajectory(
#         self,
#         target_steps: List[int],
#         move_type: str = "joint",
#         move_time: float = 2.0,
#         tcp_speed: float = 50.0,
#         target_pose: np.ndarray | None = None,
#     ):
#         """Berechnet und sendet Trajektorie"""
#         if self._traj_state != "IDLE":
#             print("Error: Trajectory already in progress")
#             return

#         current_steps = shared_data.get_steps()
#         if current_steps is None:
#             print("Error: No current position")
#             return

#         print(f"Calculating {move_type} trajectory...")

#         # Trajektorie berechnen
#         if move_type == "joint":
#             self._traj_points = self.traj_calc.calculate_joint_trajectory(
#                 current_steps, target_steps, move_time
#             )
#         elif move_type == "cartesian" and target_pose is not None:
#             self._traj_points = self.traj_calc.calculate_cartesian_trajectory(
#                 current_steps, target_pose, tcp_speed
#             )
#         else:
#             print("Error: Invalid parameters for trajectory planning")
#             return

#         if not self._traj_points:
#             print("Error: No trajectory points calculated")
#             return

#         print(f"Trajectory calculated: {len(self._traj_points)} points")

#         # Trajektorie senden starten
#         self._start_trajectory_send()

#     def _start_trajectory_send(self):
#         """Startet den Sende-Prozess"""
#         if not self._traj_points:
#             return

#         total_time_ms = self._traj_points[-1][0]

#         # CMD_TRAJ_START senden
#         start_data = struct.pack("<II", len(self._traj_points), total_time_ms)
#         packet = packet_builder.build_packet(CMD_TRAJ_START, start_data)

#         shared_data.set_data_out(packet)
#         self._traj_state = "WAIT_START_ACK"
#         self._last_ack_time = self._get_current_time()

#         print("Sent: CMD_TRAJ_START")

#     def on_data_in(self, value: int):
#         """Verarbeitet eingehende Daten (ACKs)"""
#         super().on_data_in(value)  # Original-Methode aufrufen

#         # Trajektorien-ACKs verarbeiten
#         if self._traj_state == "WAIT_START_ACK":
#             if value == INFO_TRAJ_START_ACK:
#                 print("Received: INFO_TRAJ_START_ACK")
#                 self._current_chunk = 0
#                 self._send_next_chunk()

#         elif self._traj_state == "WAIT_CHUNK_ACK":
#             if value == INFO_TRAJ_CHUNK_ACK:
#                 print("Received: INFO_TRAJ_CHUNK_ACK")
#                 self._send_next_chunk()

#         elif self._traj_state == "WAIT_EXEC_ACK":
#             if value == INFO_TRAJ_EXEC_ACK:
#                 print("Received: TRAJ_EXEC_ACK - Trajectory execution started")
#                 self._traj_state = "IDLE"
#                 self._traj_points = []
#                 self._current_chunk = 0

#     def _send_next_chunk(self):
#         """Sendet den nächsten Chunk"""
#         if not self._traj_points:
#             return

#         # Berechne Start- und Endindex für diesen Chunk
#         start_idx = self._current_chunk * self._chunk_size
#         end_idx = min(start_idx + self._chunk_size, len(self._traj_points))

#         if start_idx >= len(self._traj_points):
#             # Alle Chunks gesendet -> CMD_TRAJ_EXEC senden
#             packet = packet_builder.build_packet(0x14, b"")  # CMD_TRAJ_EXEC
#             shared_data.set_data_out(packet)
#             self._traj_state = "WAIT_EXEC_ACK"
#             self._last_ack_time = self._get_current_time()
#             print("Sent: CMD_TRAJ_EXEC")
#             return

#         # Chunk-Daten bauen
#         chunk_data = bytearray()
#         chunk_data.append(self._current_chunk)  # Chunk ID

#         for i in range(start_idx, end_idx):
#             time_ms, steps = self._traj_points[i]
#             chunk_data.extend(struct.pack("<I6i", time_ms, *steps))

#         # Paket senden
#         packet = packet_builder.build_packet(CMD_TRAJ_DATA, chunk_data)  # CMD_TRAJ_DATA
#         shared_data.set_data_out(packet)
#         self._traj_state = "WAIT_CHUNK_ACK"
#         self._last_ack_time = self._get_current_time()

#         print(f"Sent: CMD_TRAJ_DATA chunk {self._current_chunk}")
#         self._current_chunk += 1

#     def update(self):
#         """Wird regelmäßig vom MotionPlannerThread aufgerufen"""
#         current_time = self._get_current_time()

#         # Timeout für Trajektorien-ACKs
#         if self._traj_state != "IDLE":
#             if current_time - self._last_ack_time > self._ack_timeout_ms:
#                 print(f"Error: Trajectory ACK timeout in state {self._traj_state}")
#                 self._traj_state = "IDLE"
#                 self._traj_points = []
#                 self._current_chunk = 0

#     def _get_current_time(self):
#         """Hilfsfunktion für Zeit in ms"""
#         import time

#         return int(time.time() * 1000)

# def jog_cart(self):
#     cycle_ms = 5
#     dt = cycle_ms / 1000.0
#     lam = 0.03

#     is_cart_jog_active = shared_data.get_is_cart_jog_active()
#     cart_jog_dir = shared_data.get_cart_jog_dir()

#     if not is_cart_jog_active or not cart_jog_dir:
#         self._dq_accum[:] = 0.0
#         return

#     speed = shared_data.get_cart_jog_speed()
#     frame = "WRF"

#     steps = shared_data.get_steps()
#     # steps = [40250, 42760, -11820, -24080, -16438, -6400]
#     if not steps:
#         return

#     meas = np.array(steps, dtype=int)
#     q = self._steps_to_q_rad(meas.tolist())
#     T = self.R.fkine(q)

#     # Cartesian twist berechnen
#     xi = self._build_twist(cart_jog_dir, speed, T, frame=frame)

#     # Jacobian und inverse Kinematik
#     J = self.R.jacob0(q)
#     try:
#         qdot = self._dls_qdot(J, xi, lam=lam)  # rad/s
#     except np.linalg.LinAlgError:
#         return

#     # Joint velocity zu Joint increment
#     dq = qdot * dt  # rad/Tick

#     # Hier ist der wichtige Teil: Steps mit Scaling und Akkumulator
#     dsteps = self._compute_step_increments(dq)

#     if dsteps is not None:
#         # Motor direction mapping anwenden
#         dsteps = (self.motor_sign * dsteps).astype(int)
#         packet = packet_builder.build_packet(CMD_JOG_CART, dsteps.tolist())
#         shared_data.set_data_out(packet)

# def _compute_step_increments(
#     self, dq: npt.NDArray[np.float64]
# ) -> npt.NDArray[np.int_] | None:
#     """
#     Berechnet Step-Inkremente mit intelligentem Scaling um saubere Bewegungen zu garantieren.
#     Gibt nur dann Steps aus, wenn mindestens eine Achse die Mindestschrittgröße erreicht.
#     """
#     # 1. dq zu Steps konvertieren (ohne Rundung)
#     step_rad = np.array([2 * np.pi / self.R._steps_per_rev(i) for i in range(6)])
#     steps_float = dq / step_rad

#     # 2. Akkumulator updaten
#     self._dq_accum += dq

#     # 3. Prüfen ob genug "Material" für eine sinnvolle Ausgabe da ist
#     accum_steps_float = self._dq_accum / step_rad
#     max_abs_accum = np.max(np.abs(accum_steps_float))

#     # Erst ausgeben wenn mindestens eine Achse die Mindestschrittgröße erreicht
#     if max_abs_accum < self.min_step_size:
#         return None

#     # 4. Scaling berechnen um in den gewünschten Bereich zu kommen
#     if max_abs_accum < self.target_step_size:
#         # Zu klein -> hochskalieren auf Zielgröße
#         scale = self.target_step_size / max_abs_accum
#     elif max_abs_accum > self.max_step_size:
#         # Zu groß -> runterskalieren auf Maximum
#         scale = self.max_step_size / max_abs_accum
#     else:
#         # Im gewünschten Bereich -> keine Skalierung
#         scale = 1.0

#     # 5. Skalierte Steps berechnen
#     scaled_steps_float = accum_steps_float * scale

#     # 6. Zu Integer runden
#     dsteps = np.round(scaled_steps_float).astype(int)

#     # 7. Verbrauchte Winkel vom Akkumulator abziehen
#     consumed_dq = (
#         dsteps * step_rad / scale
#     )  # Rücktransformation der tatsächlich verbrauchten Winkel
#     self._dq_accum -= consumed_dq

#     return dsteps

# def _build_twist(
#     self,
#     cart_dir: Sequence[float],
#     speed: float,
#     T_now: SE3,
#     frame: Literal["WRF", "TRF"] = "TRF",
# ) -> npt.NDArray[np.float64]:
#     """
#     Erstellt einen Cartesian twist vector aus Richtung und Geschwindigkeit.
#     """
#     d = np.asarray(cart_dir, dtype=float)
#     lin_dir, ang_dir = d[:3], d[3:]

#     # Geschwindigkeits-Mapping (kannst du anpassen)
#     v = lin_dir * (0.008 * speed)  # m/s (etwas größer für spürbare Bewegung)
#     w = np.deg2rad(ang_dir * (1.5 * speed))  # rad/s

#     if frame == "TRF":
#         # Tool frame -> Base frame transformation
#         R = T_now.R
#         v = R @ v
#         w = R @ w

#     return np.hstack([v, w])

# def _steps_to_q_rad(self, steps: list[int]) -> npt.NDArray[np.float64]:
#     """Konvertiert Steps zu Joint angles in Radians."""
#     degs: list[float] = self.R.get_joint_angles_deg(steps)
#     return np.deg2rad(np.array(degs, dtype=float))

# def _dls_qdot(
#     self,
#     J: npt.NDArray[np.float64],
#     xi: npt.NDArray[np.float64],
#     lam: float = 0.03,
# ) -> npt.NDArray[np.float64]:
#     """
#     Damped Least Squares inverse kinematics.
#     """
#     JT = J.T
#     JJt = J @ JT
#     return JT @ np.linalg.inv(JJt + (lam**2) * np.eye(6)) @ xi
