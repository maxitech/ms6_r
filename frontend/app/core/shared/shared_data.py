import threading
from queue import Queue, Empty
from typing import Dict


class SharedData:
    def __init__(self):
        self._data_queue_out: Queue = Queue()
        self._lock = threading.RLock()

        self._data_out: bytes | None = None
        self._data_in: int | None = None
        self._curr_steps: list[int] | None = None
        self._sequence: list[list[int]] = []
        self._run_sequence_flag: bool = False

        # Trajektorien-Anfragen
        self._traj_request_queue = Queue()
        # self._signals["traj_request"] = []

        # *** Cart Jog ***
        self._is_cart_jog_active: bool = False
        self._cart_jog_dir: list[int] | None = None
        self._cart_jog_speed: int = 0

        self._signals: Dict[str, list] = {
            "new_steps": [],
            "data_in": [],
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

    def set_data_out(self, data: bytes) -> None:
        with self._lock:
            self._data_out = data
            self._data_queue_out.put(data)

    def clear_data_queue_out(self) -> None:
        with self._lock:
            while True:
                try:
                    self._data_queue_out.get_nowait()
                except Empty:
                    break

    def get_data_out(self) -> bytes | None:
        with self._lock:
            return self._data_out

    def get_next_data(self) -> bytes | None:
        with self._lock:
            return self._data_queue_out.get_nowait()

    def update_data_in(self, data: int) -> None:
        with self._lock:
            self._data_in = data
        self._emit_signal("data_in", data)

    def get_data_in(self) -> int | None:
        with self._lock:
            return self._data_in

    def update_steps(self, steps: list[int]):
        with self._lock:
            self._curr_steps = steps
        self._emit_signal("new_steps", steps)

    def get_steps(self) -> list[int] | None:
        with self._lock:
            return self._curr_steps

    def add_position_to_sequence(self, pos: list[int]) -> None:
        with self._lock:
            self._sequence.append(pos)

    def del_position_from_sequence(self) -> None:
        with self._lock:
            self._sequence.pop()

    def get_sequence(self) -> list[list[int]]:
        with self._lock:
            return self._sequence

    def set_is_run_sequence(self, is_run: bool) -> None:
        with self._lock:
            self._run_sequence_flag = is_run

    def is_run_sequence(self) -> bool:
        with self._lock:
            return self._run_sequence_flag

    def request_trajectory(self, request_data: dict):
        """Stellt eine Trajektorien-Anfrage"""
        with self._lock:
            self._traj_request_queue.put(request_data)
        self._emit_signal("traj_request", request_data)

    def get_traj_request(self):
        """Holt die nächste Trajektorien-Anfrage"""
        with self._lock:
            try:
                return self._traj_request_queue.get_nowait()
            except Empty:
                return None

    #  *** Cart Jog ***
    def set_is_cart_jog_active(self, is_active: bool) -> None:
        with self._lock:
            self._is_cart_jog_active = is_active

    def get_is_cart_jog_active(self) -> bool:
        with self._lock:
            return self._is_cart_jog_active

    def set_cart_jog_dir(self, cart_jog_dir: list[int]) -> None:
        with self._lock:
            self._cart_jog_dir = cart_jog_dir

    def get_cart_jog_dir(self) -> list[int] | None:
        with self._lock:
            return self._cart_jog_dir

    def set_cart_jog_speed(self, speed: int) -> None:
        with self._lock:
            self._cart_jog_speed = speed

    def get_cart_jog_speed(self) -> int:
        with self._lock:
            return self._cart_jog_speed

    # *** Private ***
    def _emit_signal(self, signal_name, data):
        for callback in self._signals[signal_name]:
            callback(data)


shared_data = SharedData()
