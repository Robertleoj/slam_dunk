from __future__ import annotations
from pathlib import Path
import threading
import subprocess
from sys import argv

from .bindings import (
    __doc__,
    Visualizer as Visualizer_internal,
    Canvas,
    Scene,
    geom,
    geom2d,
    spawn_window as spawn_window_internal,
)


def _executable_path():
    executable_path = Path(__file__).parent / "slamd_window"

    if not executable_path.exists():
        print("Executable path not found! Assuming dev install, passing None")
        executable_path = None

    return executable_path


class Visualizer:
    def __init__(self, name: str, spawn=True, port: int = 5555) -> None:
        self._impl = Visualizer_internal(name, port)

        if spawn:
            spawn_window(port)

    def hang_forever(self):
        threading.Event().wait()

    def add_scene(self, name, scene):
        return self._impl.add_scene(name, scene)

    def add_canvas(self, name, canvas):
        return self._impl.add_canvas(name, canvas)


def spawn_window(port: int = 5555) -> None:
    executable_path = _executable_path()

    spawn_window_internal(
        port,
        str(executable_path) if executable_path is not None else None,
    )


def _window_cli():
    exe_path = _executable_path()

    if exe_path is None:
        raise RuntimeError("Can't find exe path")

    subprocess.run([exe_path, *argv[1:]])


__all__ = [
    "__doc__",
    "Visualizer",
    "Canvas",
    "Scene",
    "geom",
    "geom2d",
    "spawn_window",
    "_window_cli",
]
