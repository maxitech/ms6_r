from PySide6.QtCore import QTimer
from app.core.serial_connection import SerialConnection


class MainWindowController:
    def __init__(self, ui):
        self._ui = ui
        self._serial = SerialConnection()
        self._current_ports = []

        self._check_ports()
        self._port_timer = QTimer()
        self._port_timer.timeout.connect(self._check_ports)
        self._port_timer.start(2000)

    # private methods
    def _check_ports(self):
        _ports = self._serial.getPorts()
        if _ports != self._current_ports:
            self._current_ports = _ports
            self._update_combo_box()

    def _update_combo_box(self):
        self._ui.con_device_comboBox.clear()
        self._ui.con_device_comboBox.addItems(self._current_ports)
