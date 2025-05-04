import numpy as np
from .bindings.geom import (
    Box,
    Arrows,
    CameraFrustum,
    PointCloud as PointCloud_internal,
    PolyLine as PolyLine_internal,
    Mesh,
    Sphere,
    Triad,
)
from .utils.colors import Color
from .utils.handle_input import process_color, process_radii


def PointCloud(
    positions: np.ndarray,
    colors: np.ndarray | tuple[int, int, int] = Color.black,
    radii: np.ndarray | float = 1.0,
    min_brightness: float = 1.0,
):
    n = positions.shape[0]
    colors_np = process_color(colors, n)
    radii_np = process_radii(radii, n)

    return PointCloud_internal(positions, colors_np, radii_np, min_brightness)


def PolyLine(
    points: np.ndarray,
    thickness: float = 1.0,
    color: np.ndarray | tuple[int, int, int] = Color.red,
):
    color_np = process_color(color, 1).reshape(3)
    return PolyLine_internal(points, thickness, color_np)


__all__ = [
    "Box",
    "Arrows",
    "CameraFrustum",
    "PointCloud",
    "PolyLine",
    "Mesh",
    "Sphere",
    "Triad",
]
