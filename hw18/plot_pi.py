"""
PI current controller tuning plotter for HW18 haptic paddle.

Usage:
    python plot_pi.py [COM_PORT]

    COM_PORT defaults to COM3 on Windows or /dev/ttyACM0 on Linux/Mac.
    Override from the command line, e.g.:
        python plot_pi.py COM5
        python plot_pi.py /dev/ttyACM1

Workflow:
    1. Flash the Pico, open this script (close any other serial monitor first).
    2. Run the script. It sends 'a' automatically and waits for the sweep.
    3. A plot of desired vs. actual current appears with annotations.
    4. Close the plot window to run another sweep (or Ctrl-C to quit).
"""

import sys
import time
import platform
import serial
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

# ── configuration ────────────────────────────────────────────────────────────
BAUD       = 115200
NSAMP      = 400        # must match #define NSAMP in 18_paddle.c
TIMEOUT_S  = 15         # max seconds to wait for all samples

if sys.argv[1:]:
    PORT = sys.argv[1]
elif platform.system() == "Windows":
    PORT = "COM7"
else:
    PORT = "/dev/ttyACM0"
# ─────────────────────────────────────────────────────────────────────────────


def open_port(port: str) -> serial.Serial:
    while True:
        try:
            s = serial.Serial(port, BAUD, timeout=2)
            print(f"Connected to {s.name}")
            return s
        except serial.SerialException:
            print(f"Waiting for {port} ...")
            time.sleep(1)


def run_sweep(ser: serial.Serial):
    """Send 'a', then collect exactly NSAMP data lines; return (idx, desired, actual)."""
    ser.reset_input_buffer()
    ser.write(b'a')
    print("Sweep started — waiting for data ...")

    idx, desired, actual = [], [], []
    deadline = time.monotonic() + TIMEOUT_S

    while len(idx) < NSAMP:
        if time.monotonic() > deadline:
            print(f"Timeout: only got {len(idx)}/{NSAMP} samples.")
            break
        raw = ser.readline().decode("utf-8", errors="replace").strip()
        if not raw:
            continue
        parts = raw.split()
        if len(parts) == 3:
            try:
                idx.append(int(parts[0]))
                desired.append(float(parts[1]))
                actual.append(float(parts[2]))
            except ValueError:
                pass   # skip prompt / non-data lines

    return np.array(idx), np.array(desired), np.array(actual)


def annotate_step(ax, idx, actual, desired_level, start_idx, label_prefix):
    """Annotate rise time, overshoot, and steady-state error for one step."""
    target = desired_level
    # steady-state: mean of last quarter of the step window
    end   = start_idx + 100
    ss    = float(np.mean(actual[start_idx + 75 : end]))
    ss_err = abs(target - ss)

    # rise time: 10% -> 90% of step amplitude
    lo, hi = min(actual[start_idx], target), max(actual[start_idx], target)
    amp = hi - lo
    if amp == 0:
        return
    t10 = next((i for i, v in enumerate(actual[start_idx:end]) if abs(v - lo) >= 0.1 * amp), None)
    t90 = next((i for i, v in enumerate(actual[start_idx:end]) if abs(v - lo) >= 0.9 * amp), None)
    rise = (t90 - t10) if (t10 is not None and t90 is not None) else None

    overshoot = max((actual[start_idx:end] - target) / abs(target) * 100) if target > 0 \
           else max((target - actual[start_idx:end]) / abs(target) * 100)
    overshoot = max(overshoot, 0.0)

    x_label = idx[start_idx + 50]
    y_label = target * 1.15
    txt = f"{label_prefix}\nss_err={ss_err:.1f} mA\novershoot={overshoot:.1f}%"
    if rise is not None:
        txt += f"\nrise={rise} ms"
    ax.annotate(txt, xy=(x_label, y_label), fontsize=7,
                bbox=dict(boxstyle="round,pad=0.3", fc="lightyellow", alpha=0.8))


def plot(idx, desired, actual):
    fig, ax = plt.subplots(figsize=(10, 5))
    ax.plot(idx, desired, "k--", linewidth=1.2, label="desired (mA)")
    ax.plot(idx, actual,  "tab:blue", linewidth=1.5, label="actual (mA)")
    ax.axhline(0, color="gray", linewidth=0.5)

    # annotate the first two positive and negative steps (at ~0, 100, 200, 300 ms)
    step_starts = [0, 100, 200, 300]
    labels      = ["+step 1", "-step 1", "+step 2", "-step 2"]
    targets     = [desired[s + 5] for s in step_starts]
    for s, lab, tgt in zip(step_starts, labels, targets):
        annotate_step(ax, idx, actual, tgt, s, lab)

    ax.set_xlabel("Time (ms)")
    ax.set_ylabel("Current (mA)")
    ax.set_title("PI Current Controller — Step Response\n"
                 "Tune kp (rise speed) then ki (steady-state error)")
    ax.legend(loc="upper right")
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    plt.show()


def main():
    ser = open_port(PORT)
    try:
        while True:
            idx, desired, actual = run_sweep(ser)
            if len(idx) == 0:
                print("No data received. Is the Pico running?")
            else:
                plot(idx, desired, actual)
            ans = input("Run another sweep? [Y/n]: ").strip().lower()
            if ans == "n":
                break
    finally:
        ser.close()


if __name__ == "__main__":
    main()
