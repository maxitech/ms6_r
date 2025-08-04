from PySide6.QtCore import QThread
from queue import Queue, Empty
import serial
from oclock import Timer

from app.core.shared.shared_data import shared_data


class SerialWriterThread(QThread):
    INTERVAL_S = 0.01  # 10ms

    def __init__(self, serial: serial.Serial):
        super().__init__()
        self._serial_port = serial
        self._is_running = True

    def run(self):
        print("SerialWriter started!")
        tmr = Timer(interval=self.INTERVAL_S, precise=True)
        while self._is_running:
            try:
                data = shared_data.get_next_data()
            except Empty:
                data = None

            if data and self._serial_port:
                try:
                    self._serial_port.write(data)
                except serial.SerialException as e:
                    print(f"Serial error: {e}. Stopping writer thread.")
                    self.stop()
                except Exception as e:
                    print(f"Unhandled error: {e}")
                    self.stop()

            tmr.checkpt()

    def stop(self):
        self._is_running = False
        if QThread.currentThread() != self and self.isRunning():
            self.wait()
