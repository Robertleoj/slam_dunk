import numpy as np
from ..bindings.geom import (
    PointCloud as PointCloud_internal,
    PolyLine as PolyLine_internal,
    Sphere as Sphere_internal,
    Arrows as Arrows_internal,
)
from .._utils.colors import Color
from .._utils.handle_input import process_color, process_radii, process_single_color


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
    min_brightness: float = 1.0,
):
    color_np = process_single_color(color)
    return PolyLine_internal(points, thickness, color_np, min_brightness)


def Sphere(radius: float, color: np.ndarray | tuple[int, int, int] = Color.blue):
    return Sphere_internal(radius, process_single_color(color))


def Arrows(
    starts: np.ndarray,
    ends: np.ndarray,
    colors: np.ndarray | tuple[int, int, int] = Color.dark_red,
    thickness: float = 0.5,
):
    return Arrows_internal(
        starts, ends, process_color(colors, starts.shape[0]), thickness
    )
