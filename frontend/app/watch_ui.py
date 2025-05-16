import os
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

UI_DIR = "ui"
UI_GEN_DIR = "ui_gen"

os.makedirs(UI_DIR, exist_ok=True)
os.makedirs(UI_GEN_DIR, exist_ok=True)


class UiFileHandler(FileSystemEventHandler):
    def on_any_event(self, event):
        if event.is_directory or not event.src_path.endswith(".ui"):
            return
        self.convert_ui_to_py(event.src_path)

    def convert_ui_to_py(self, ui_path):
        filename = os.path.basename(ui_path)
        name, _ = os.path.splitext(filename)
        output_path = os.path.join(UI_GEN_DIR, f"{name}_ui.py")
        try:
            subprocess.run(["pyside6-uic", "-o", output_path, ui_path], check=True)
            print(f"[✔] Converted {filename} -> {output_path}")
        except subprocess.CalledProcessError as e:
            print(f"[✖] Error converting {filename}: {e}")


if __name__ == "__main__":
    print(f"[🔍] Watching '{UI_DIR}/' for .ui changes...")

    if not any(fname.endswith(".ui") for fname in os.listdir(UI_DIR)):
        print("[ℹ] No .ui files found in 'ui/' – waiting for files...")

    event_handler = UiFileHandler()
    observer = Observer()
    observer.schedule(event_handler, path=UI_DIR, recursive=False)
    observer.start()

    try:
        while True:
            pass
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
