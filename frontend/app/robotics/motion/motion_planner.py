from typing import Literal, Sequence
import numpy as np
import numpy.typing as npt
from spatialmath import SE3

from app.robotics.robot import Robot
from app.constants.ms6_r_constants import MS6_R_CONSTANTS
from app.core.shared.shared_data import shared_data
from app.core.packet_builder import packet_builder
from app.constants.com_protocol import CMD_JOG_CART


class MotionPlanner:
    def __init__(self):
        self.R = Robot(constants=MS6_R_CONSTANTS)
        self._dq_accum = np.zeros(6)

        # Motor direction mapping (+1: +Steps = CCW, -1: +Steps = CW)
        self.motor_sign = np.array([-1, -1, -1, -1, +1, -1], dtype=int)

        # Jog parameters - diese kannst du anpassen
        self.min_step_size = 20  # Minimum Steps pro Ausgabe
        self.target_step_size = 40  # Ziel Steps pro Ausgabe
        self.max_step_size = 80  # Maximum Steps pro Ausgabe

    def log(self, data):
        print("Log: ", data)

    def jog_cart(self):
        cycle_ms = 5
        dt = cycle_ms / 1000.0
        lam = 0.03

        is_cart_jog_active = shared_data.get_is_cart_jog_active()
        cart_jog_dir = shared_data.get_cart_jog_dir()

        if not is_cart_jog_active or not cart_jog_dir:
            self._dq_accum[:] = 0.0
            return

        speed = shared_data.get_cart_jog_speed()
        frame = "WRF"

        steps = shared_data.get_steps()
        # steps = [40250, 42760, -11820, -24080, -16438, -6400]
        if not steps:
            return

        meas = np.array(steps, dtype=int)
        q = self._steps_to_q_rad(meas.tolist())
        T = self.R.fkine(q)

        # Cartesian twist berechnen
        xi = self._build_twist(cart_jog_dir, speed, T, frame=frame)

        # Jacobian und inverse Kinematik
        J = self.R.jacob0(q)
        try:
            qdot = self._dls_qdot(J, xi, lam=lam)  # rad/s
        except np.linalg.LinAlgError:
            return

        # Joint velocity zu Joint increment
        dq = qdot * dt  # rad/Tick

        # Hier ist der wichtige Teil: Steps mit Scaling und Akkumulator
        dsteps = self._compute_step_increments(dq)

        if dsteps is not None:
            # Motor direction mapping anwenden
            dsteps = (self.motor_sign * dsteps).astype(int)
            packet = packet_builder.build_packet(CMD_JOG_CART, dsteps.tolist())
            shared_data.set_data_out(packet)

    def _compute_step_increments(
        self, dq: npt.NDArray[np.float64]
    ) -> npt.NDArray[np.int_] | None:
        """
        Berechnet Step-Inkremente mit intelligentem Scaling um saubere Bewegungen zu garantieren.
        Gibt nur dann Steps aus, wenn mindestens eine Achse die Mindestschrittgröße erreicht.
        """
        # 1. dq zu Steps konvertieren (ohne Rundung)
        step_rad = np.array([2 * np.pi / self.R._steps_per_rev(i) for i in range(6)])
        steps_float = dq / step_rad

        # 2. Akkumulator updaten
        self._dq_accum += dq

        # 3. Prüfen ob genug "Material" für eine sinnvolle Ausgabe da ist
        accum_steps_float = self._dq_accum / step_rad
        max_abs_accum = np.max(np.abs(accum_steps_float))

        # Erst ausgeben wenn mindestens eine Achse die Mindestschrittgröße erreicht
        if max_abs_accum < self.min_step_size:
            return None

        # 4. Scaling berechnen um in den gewünschten Bereich zu kommen
        if max_abs_accum < self.target_step_size:
            # Zu klein -> hochskalieren auf Zielgröße
            scale = self.target_step_size / max_abs_accum
        elif max_abs_accum > self.max_step_size:
            # Zu groß -> runterskalieren auf Maximum
            scale = self.max_step_size / max_abs_accum
        else:
            # Im gewünschten Bereich -> keine Skalierung
            scale = 1.0

        # 5. Skalierte Steps berechnen
        scaled_steps_float = accum_steps_float * scale

        # 6. Zu Integer runden
        dsteps = np.round(scaled_steps_float).astype(int)

        # 7. Verbrauchte Winkel vom Akkumulator abziehen
        consumed_dq = (
            dsteps * step_rad / scale
        )  # Rücktransformation der tatsächlich verbrauchten Winkel
        self._dq_accum -= consumed_dq

        return dsteps

    def _build_twist(
        self,
        cart_dir: Sequence[float],
        speed: float,
        T_now: SE3,
        frame: Literal["WRF", "TRF"] = "TRF",
    ) -> npt.NDArray[np.float64]:
        """
        Erstellt einen Cartesian twist vector aus Richtung und Geschwindigkeit.
        """
        d = np.asarray(cart_dir, dtype=float)
        lin_dir, ang_dir = d[:3], d[3:]

        # Geschwindigkeits-Mapping (kannst du anpassen)
        v = lin_dir * (0.008 * speed)  # m/s (etwas größer für spürbare Bewegung)
        w = np.deg2rad(ang_dir * (1.5 * speed))  # rad/s

        if frame == "TRF":
            # Tool frame -> Base frame transformation
            R = T_now.R
            v = R @ v
            w = R @ w

        return np.hstack([v, w])

    def _steps_to_q_rad(self, steps: list[int]) -> npt.NDArray[np.float64]:
        """Konvertiert Steps zu Joint angles in Radians."""
        degs: list[float] = self.R.get_joint_angles_deg(steps)
        return np.deg2rad(np.array(degs, dtype=float))

    def _dls_qdot(
        self,
        J: npt.NDArray[np.float64],
        xi: npt.NDArray[np.float64],
        lam: float = 0.03,
    ) -> npt.NDArray[np.float64]:
        """
        Damped Least Squares inverse kinematics.
        """
        JT = J.T
        JJt = J @ JT
        return JT @ np.linalg.inv(JJt + (lam**2) * np.eye(6)) @ xi
