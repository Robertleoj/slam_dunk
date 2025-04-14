from __future__ import annotations
import bindings._geom
import numpy
import typing
__all__ = ['image', 'points_2d', 'poly_line']
def image(image: numpy.ndarray) -> bindings._geom.Image:
    """
    Create an Image geometry from a NumPy uint8 array (H, W, C)
    """
@typing.overload
def points_2d(positions: numpy.ndarray, color: numpy.ndarray, radius: float) -> bindings._geom.Points2D:
    """
    Create 2D points with uniform color and radius
    """
@typing.overload
def points_2d(positions: numpy.ndarray, colors: numpy.ndarray, radii: list[float] | numpy.ndarray) -> bindings._geom.Points2D:
    """
    Create 2D points with per-point color and radius
    """
def poly_line(points: numpy.ndarray, color: numpy.ndarray, thickness: float) -> bindings._geom.PolyLine2D:
    """
    Create a 2D poly line
    """
