from PySide6.QtCore import QThread, Signal
import serial
import time


class SerialReaderThread(QThread):
    data_received = Signal(bytes)  # signal for sending data to ui

    def __init__(self, serial: serial.Serial):
        super().__init__()
        self._serial_port = serial
        self._is_running = True

    def run(self):
        print("SerialWorker started!")
        while self._is_running:
            try:
                if self._serial_port:
                    n = self._serial_port.in_waiting
                    if n > 0:
                        data = self._serial_port.read(n)
                        self.data_received.emit(data)  # send data to ui
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                self.stop()
            except Exception as e:
                print(f"Unhandled error: {e}")
                self.stop()
            QThread.msleep(1)  # reduce cpu power

    def stop(self):
        self._is_running = False
        if QThread.currentThread() != self and self.isRunning():
            self.wait()
