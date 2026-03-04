from PySide6.QtCore import QThread, Signal, Slot
import serial
import time
from app.core.shared.shared_data import shared_data
from app.robotics.motion.motion_planner import EnhancedMotionPlanner


class MotionPlannerThread(QThread):

    def __init__(self, serial: serial.Serial):
        super().__init__()
        # self._m_plr = MotionPlanner()
        self._m_plr = EnhancedMotionPlanner()
        self._serial_port = serial
        self._is_running = True

        # shared_data.subscribe("new_steps", lambda data: print("MotionPlanner: ", data))
        shared_data.subscribe("new_steps", self._m_plr.log)
        shared_data.subscribe("data_in", self._m_plr.on_data_in)

    #     shared_data.subscribe("traj_request", self._handle_traj_request)

    # def _handle_traj_request(self, request):
    #     """Verarbeitet Trajektorien-Anfragen"""
    #     # request ist ein Dict mit: type, target, move_time, etc.
    #     if request["type"] == "joint":
    #         self._m_plr.plan_and_send_trajectory(
    #             target_steps=request["target_steps"],
    #             move_type="joint",
    #             move_time=request.get("move_time", 2.0),
    #         )
    #     elif request["type"] == "cartesian":
    #         self._m_plr.plan_and_send_trajectory(
    #             target_steps=[],  # Nicht benötigt für cartesian
    #             move_type="cartesian",
    #             tcp_speed=request.get("tcp_speed", 50.0),
    #             target_pose=request["target_pose"],
    #         )

    def run(self):
        cycle_ms = 10
        print("MotionPlanner started!")

        while self._is_running:
            if not self._serial_port:
                self.stop()

            if shared_data.is_run_sequence():
                shared_data.set_is_run_sequence(False)
                seq = shared_data.get_sequence()
                # self._m_plr.start_sequence(seq)

                self._m_plr.start_trajectory_sequence(seq, move_time=10.0)

            # if shared_data.get_is_cart_jog_active():
            #     self._m_plr.jog_cart()

            self._m_plr.update()

            QThread.msleep(cycle_ms)

    def stop(self):
        self._is_running = False
        if QThread.currentThread() != self and self.isRunning():
            self.wait()
