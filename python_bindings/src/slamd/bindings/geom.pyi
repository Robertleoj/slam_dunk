from __future__ import annotations
import bindings._geom
import numpy
import typing
__all__ = ['arrows', 'box', 'camera_frustum', 'mono_mesh', 'point_cloud', 'poly_line', 'simple_mesh', 'sphere', 'triad']
def arrows(starts: numpy.ndarray, ends: numpy.ndarray, colors: numpy.ndarray, thickness: float) -> bindings._geom.Arrows:
    """
    Create an Arrows geometry
    """
def box() -> bindings._geom.Box:
    """
    Create a Box geometry
    """
def camera_frustum(*args, **kwargs) -> bindings._geom.CameraFrustum:
    """
    Create a CameraFrustum geometry
    """
def mono_mesh(vertices: numpy.ndarray, triangle_indices: list[int], color: numpy.ndarray) -> bindings._geom.MonoMesh:
    """
    Create a MonoMesh geometry
    """
@typing.overload
def point_cloud(positions: numpy.ndarray, color: numpy.ndarray, radius: float) -> bindings._geom.PointCloud:
    """
    Create a PointCloud with uniform color and radius
    """
@typing.overload
def point_cloud(positions: numpy.ndarray, colors: numpy.ndarray, radii: list[float]) -> bindings._geom.PointCloud:
    """
    Create a PointCloud with per-point color and radius
    """
def poly_line(points: numpy.ndarray, thickness: float, color: numpy.ndarray) -> bindings._geom.PolyLine:
    """
    Create a PolyLine geometry
    """
def simple_mesh(vertices: numpy.ndarray, vertex_colors: numpy.ndarray, triangle_indices: list[int]) -> bindings._geom.SimpleMesh:
    """
    Create a SimpleMesh geometry from raw data
    """
def sphere(radius: float = 1.0, color: numpy.ndarray = ...) -> bindings._geom.Sphere:
    """
    Create a Sphere geometry
    """
def triad(scale: float = 1.0, thickness: float = 0.10000000149011612) -> bindings._geom.Triad:
    """
    Create a Triad geometry
    """
