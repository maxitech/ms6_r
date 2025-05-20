from PySide6.QtCore import QThread, Signal
import serial
import time


class SerialWorker(QThread):
    data_received = Signal(str)  # signal for sending data to ui

    def __init__(self, serial):
        super().__init__()
        self._serial_port = serial
        self._running = True

    def run(self):
        while self._running:
            try:
                if self._serial_port and self._serial_port.in_waiting:
                    data = self._serial_port.read(self._serial_port.in_waiting).decode(
                        "utf-8"
                    )
                    self.data_received.emit(data)  # send data to ui
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                self._running = False  # stop thread if critical error occoures
                self.quit()
            except Exception as e:
                print(f"Serial error: {e}")
            time.sleep(0.01)  # reduce cpu power

    def stop(self):
        self._running = False
        self.quit()
        self.wait()
