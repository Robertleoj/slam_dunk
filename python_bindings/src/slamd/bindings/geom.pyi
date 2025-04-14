from __future__ import annotations
import bindings._geom
import numpy
import typing
__all__ = ['Arrows', 'Box', 'CameraFrustum', 'Mesh', 'PointCloud', 'PolyLine', 'Sphere', 'Triad']
def Arrows(starts: numpy.ndarray, ends: numpy.ndarray, colors: numpy.ndarray, thickness: float) -> bindings._geom.Arrows:
    """
    Create an Arrows geometry
    """
def Box() -> bindings._geom.Box:
    """
    Create a Box geometry
    """
def CameraFrustum(intrinsics_matrix: numpy.ndarray, image_width: int, image_height: int, image: numpy.ndarray | None = None, scale: float = 1.0) -> bindings._geom.CameraFrustum:
    """
    Create a CameraFrustum geometry
    """
@typing.overload
def Mesh(vertices: numpy.ndarray, vertex_colors: numpy.ndarray, triangle_indices: list[int]) -> bindings._geom.Mesh:
    """
    Create a SimpleMesh geometry from raw data
    """
@typing.overload
def Mesh(vertices: numpy.ndarray, vertex_colors: numpy.ndarray, triangle_indices: list[int], vertex_normals: numpy.ndarray) -> bindings._geom.Mesh:
    """
    Create a SimpleMesh geometry from raw data
    """
@typing.overload
def PointCloud(positions: numpy.ndarray, color: numpy.ndarray, radius: float) -> bindings._geom.PointCloud:
    """
    Create a PointCloud with uniform color and radius
    """
@typing.overload
def PointCloud(positions: numpy.ndarray, colors: numpy.ndarray, radii: list[float] | numpy.ndarray) -> bindings._geom.PointCloud:
    """
    Create a PointCloud with per-point color and radius
    """
def PolyLine(points: numpy.ndarray, thickness: float, color: numpy.ndarray) -> bindings._geom.PolyLine:
    """
    Create a PolyLine geometry
    """
def Sphere(radius: float = 1.0, color: numpy.ndarray = ...) -> bindings._geom.Sphere:
    """
    Create a Sphere geometry
    """
def Triad(scale: float = 1.0, thickness: float = 0.10000000149011612) -> bindings._geom.Triad:
    """
    Create a Triad geometry
    """
