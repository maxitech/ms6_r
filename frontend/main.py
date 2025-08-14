import os
import sys
from PySide6.QtWidgets import QApplication, QMainWindow
from app.ui_gen.main_window_ui import Ui_MainWindow
from app.controllers.main_window_controller import MainWindowController
from app.constants.com_protocoll import CMD_IDLE, NOP
from app.core.packet_builder import PacketBuilder


os.environ["QT_STYLE_OVERRIDE"] = "Fusion"


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.controller = MainWindowController(self.ui)
        self._pb = PacketBuilder()

    def closeEvent(self, event):
        print("Closing application...")
        packet: bytes = self._pb.build_packet(cmd_id=CMD_IDLE, data=NOP)
        self.controller.set_data_out(packet)
        self.controller.serial.disconnect()
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)

    dark_stylesheet = """
    QWidget {
        background-color: #2b2b2b;
        color: #ffffff;
    }
    """

    app.setStyleSheet(dark_stylesheet)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
