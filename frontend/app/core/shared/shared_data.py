import queue
import threading


class SharedData:
    def __init__(self):
        self._data_queue_out = queue.Queue()
        self._lock = threading.RLock()

        self._data_out: bytearray = None
        self._data_in: str = None

    def set_data_out(self, data: bytearray):
        with self._lock:
            self._data_out = data
            self._data_queue_out.put(data)

    def get_next_data(self) -> bytearray:
        return self._data_queue_out.get_nowait()


shared_data = SharedData()
