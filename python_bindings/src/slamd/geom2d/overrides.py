import numpy as np

from .._utils.colors import Color
from ..bindings.geom2d import (
    Points as Points_internal,
    PolyLine as PolyLine_internal,
    Circles as Circles_internal,
)

from .._utils.handle_input import process_color, process_radii, process_single_color


def Points(
    positions: np.ndarray,
    colors: np.ndarray | tuple[int, int, int] = Color.black,
    radii: np.ndarray | float = 1.0,
):
    n = positions.shape[0]
    colors_np = process_color(colors, n)
    radii_np = process_radii(radii, n)

    return Points_internal(positions, colors_np, radii_np)


def PolyLine(
    points: np.ndarray,
    color: np.ndarray | tuple[int, int, int] = Color.pink,
    thickness: float = 1.0,
):
    return PolyLine_internal(points, process_single_color(color), thickness)


def Circles(
    positions: np.ndarray,
    colors: np.ndarray | tuple[int, int, int] = Color.dark_blue,
    radii: np.ndarray | float = 1.0,
    thickness: float = 0.1,
):
    n = positions.shape[0]

    return Circles_internal(
        positions, process_color(colors, n), process_radii(radii, n), thickness
    )
