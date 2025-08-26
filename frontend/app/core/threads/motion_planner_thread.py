from PySide6.QtCore import QThread, Signal
import serial
import time
from app.core.shared.shared_data import shared_data
from app.robotics.motion.motion_planner import MotionPlanner


class MotionPlannerThread(QThread):

    def __init__(self, serial: serial.Serial):
        super().__init__()
        self._m_plr = MotionPlanner()
        self._serial_port = serial
        self._is_running = True

        # shared_data.subscribe("new_steps", lambda data: print("MotionPlanner: ", data))
        shared_data.subscribe("new_steps", self._m_plr.log)

    def run(self):
        print("MotionPlanner started!")

        while self._is_running:
            if not self._serial_port:
                self.stop()

    def stop(self):
        self._is_running = False
        if QThread.currentThread() != self and self.isRunning():
            self.wait()
