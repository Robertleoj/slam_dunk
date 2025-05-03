from __future__ import annotations
import subprocess
from pathlib import Path
import os

from .bindings import (
    __doc__,
    Visualizer as Visualizer_internal,
    Canvas,
    Scene,
    geom,
    geom2d,
    spawn_window,
)


class Visualizer:
    def __init__(self, name: str, spawn=True) -> None:
        self._impl = Visualizer_internal(name, False)

        if spawn:
            executable_path = Path(__file__).parent / "slamd_window"
            if not executable_path.exists():
                print("Executable path not found! Assuming dev install, passing None")
                executable_path = None
            spawn_window(
                name, str(executable_path) if executable_path is not None else None
            )

    def hang_forever(self):
        return self._impl.hang_forever()

    def add_scene(self, name, scene):
        return self._impl.add_scene(name, scene)

    def add_canvas(self, name, canvas):
        return self._impl.add_canvas(name, canvas)


__all__ = ["__doc__", "Visualizer", "Canvas", "Scene", "geom", "geom2d"]
