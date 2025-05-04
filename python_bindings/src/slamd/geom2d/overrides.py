import numpy as np

from .._utils.colors import Color
from ..bindings.geom2d import Points as Points_internal

from .._utils.handle_input import process_color, process_radii


def Points(
    positions: np.ndarray,
    colors: np.ndarray | tuple[int, int, int] = Color.black,
    radii: np.ndarray | float = 1.0,
):
    n = positions.shape[0]
    colors_np = process_color(colors, n)
    radii_np = process_radii(radii, n)

    return Points_internal(positions, colors_np, radii_np)
