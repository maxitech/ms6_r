import queue
import threading


class SharedData:
    def __init__(self):
        self._data_queue_out = queue.Queue()
        self._lock = threading.RLock()

        self._data_out: bytearray = None
        self._data_in: str = None
        self._curr_steps: int = None

        self._signals = {
            "new_steps": [],
        }

    # *** Public ***
    def subscribe(self, signal_name, callback) -> None:
        if signal_name in self._signals and callback not in self._signals[signal_name]:
            self._signals[signal_name].append(callback)
        else:
            print(f"No Signal: '{signal_name}' found.")

    def unsubscribe(self) -> None:
        for key in self._signals:
            self._signals[key].clear()

    def set_data_out(self, data: bytearray) -> None:
        with self._lock:
            self._data_out = data
            self._data_queue_out.put(data)

    def get_data_out(self) -> str:
        with self._lock:
            return str(self._data_out, "utf-8")

    def get_next_data(self) -> bytearray:
        with self._lock:
            return self._data_queue_out.get_nowait()

    def set_data_in(self, data: str) -> None:
        with self._lock:
            self._data_in = data

    def get_data_in(self) -> str:
        with self._lock:
            return self._data_in

    def update_steps(self, steps: int):
        with self._lock:
            self._curr_steps = steps
        self._emit_signal("new_steps", steps)

    def get_steps(self) -> int:
        with self._lock:
            return self._curr_steps

    # *** Private ***
    def _emit_signal(self, signal_name, data):
        for callback in self._signals[signal_name]:
            callback(data)


shared_data = SharedData()
