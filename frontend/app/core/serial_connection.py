import serial.tools.list_ports


class SerialConnection:
    def __init__(self, baudrate=115200):
        self._baudrate = baudrate
        self._serial = None
        self._ports = []
        self._port = None

    # public methods
    def getPorts(self):
        self._load_ports()
        devices = [port.device for port in self._ports]
        print(f"Available ports: {devices}")
        return devices

    def setPort(self, port):
        self._port = port

    def connect(self):
        try:
            self._serial = serial.Serial(self._port, self._baudrate)
            print(f"Connected to {self._port} at {self._baudrate} baud.")
        except serial.SerialException as e:
            print(f"Error connecting to {self._port}: {e}")

    def disconnect(self):
        if self._serial and self._serial.is_open:
            self._serial.close()
            print(f"Disconnected from {self._port}.")

    def is_connected(self):
        return self._serial and self._serial.is_open

    # private methods
    def _load_ports(self):
        self._ports = serial.tools.list_ports.comports()
