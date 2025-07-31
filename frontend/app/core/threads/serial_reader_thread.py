from PySide6.QtCore import QThread, Signal
import serial
import time


class SerialReaderThread(QThread):
    data_received = Signal(str)  # signal for sending data to ui

    def __init__(self, serial: serial.Serial):
        super().__init__()
        self._serial_port = serial
        self._is_running = True

    def run(self):
        print("SerialWorker started!")
        while self._is_running:
            try:
                if self._serial_port and self._serial_port.in_waiting:
                    data = self._serial_port.readline().decode("utf-8").strip()
                    print(f"Received Data: {data}")
                    self.data_received.emit(data)  # send data to ui
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                self._is_running = False
            except Exception as e:
                print(f"Unhandled error: {e}")
                self._is_running = False

            time.sleep(0.001)  # reduce cpu power

    def stop(self):
        self._is_running = False
        self.wait()
