import queue
import threading


class SharedData:
    def __init__(self):
        self._data_queue_out = queue.Queue()
        self._lock = threading.RLock()

        self._data_out: bytearray = None
        self._data_in: str = None

        self._signals = {
            "new_steps": [],
        }

    def emit_signal(self, signal_name, data):
        for callback in self._signals[signal_name]:
            callback(data)

    def subscribe(self, signal_name, callback):
        if signal_name in self._signals:
            self._signals[signal_name].append(callback)
        else:
            print(f"No Signal: '{signal_name}' found.")

    def set_data_out(self, data: bytearray):
        with self._lock:
            self._data_out = data
            self._data_queue_out.put(data)

    def get_next_data(self) -> bytearray:
        return self._data_queue_out.get_nowait()


shared_data = SharedData()
