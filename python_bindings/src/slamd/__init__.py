from __future__ import annotations
from pathlib import Path

from .bindings import (
    __doc__,
    Visualizer as Visualizer_internal,
    Canvas,
    Scene,
    geom,
    geom2d,
    spawn_window as spawn_window_internal,
)


class Visualizer:
    def __init__(self, name: str, spawn=True, port: int = 5555) -> None:
        self._impl = Visualizer_internal(name, port)

        if spawn:
            spawn_window(name, port)

    def hang_forever(self):
        return self._impl.hang_forever()

    def add_scene(self, name, scene):
        return self._impl.add_scene(name, scene)

    def add_canvas(self, name, canvas):
        return self._impl.add_canvas(name, canvas)


def spawn_window(window_name: str, port: int = 5555) -> None:
    executable_path = Path(__file__).parent / "slamd_window"

    if not executable_path.exists():
        print("Executable path not found! Assuming dev install, passing None")
        executable_path = None

    spawn_window_internal(
        window_name,
        port,
        str(executable_path) if executable_path is not None else None,
    )


__all__ = ["__doc__", "Visualizer", "Canvas", "Scene", "geom", "geom2d", "spawn_window"]
