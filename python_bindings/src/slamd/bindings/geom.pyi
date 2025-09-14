from __future__ import annotations
import bindings._geom
import collections.abc
import numpy
import typing
__all__ = ['Arrows', 'Box', 'CameraFrustum', 'Mesh', 'Plane', 'PointCloud', 'PolyLine', 'Sphere', 'Triad']
def Arrows(starts: numpy.ndarray, ends: numpy.ndarray, colors: numpy.ndarray, thickness: typing.SupportsFloat) -> bindings._geom.Arrows:
    """
    Create an Arrows geometry
    """
def Box() -> bindings._geom.Box:
    """
    Create a Box geometry
    """
def CameraFrustum(intrinsics_matrix: numpy.ndarray, image_width: typing.SupportsInt, image_height: typing.SupportsInt, image: numpy.ndarray | None = None, scale: typing.SupportsFloat = 1.0) -> bindings._geom.CameraFrustum:
    """
    Create a CameraFrustum geometry
    """
@typing.overload
def Mesh(vertices: numpy.ndarray, vertex_colors: numpy.ndarray, triangle_indices: collections.abc.Sequence[typing.SupportsInt]) -> bindings._geom.Mesh:
    """
    Create a SimpleMesh geometry from raw data
    """
@typing.overload
def Mesh(vertices: numpy.ndarray, vertex_colors: numpy.ndarray, triangle_indices: collections.abc.Sequence[typing.SupportsInt], vertex_normals: numpy.ndarray) -> bindings._geom.Mesh:
    """
    Create a SimpleMesh geometry from raw data
    """
def Plane(normal: numpy.ndarray, point: numpy.ndarray, color: numpy.ndarray, radius: typing.SupportsFloat, alpha: typing.SupportsFloat) -> bindings._geom.Plane:
    """
    Create a Plane geometry
    """
def PointCloud(positions: numpy.ndarray, colors: numpy.ndarray, radii: list[float] | numpy.ndarray, min_brightness: typing.SupportsFloat = 1.0) -> bindings._geom.PointCloud:
    """
    Create a PointCloud with per-point color and radius
    """
def PolyLine(points: numpy.ndarray, thickness: typing.SupportsFloat, color: numpy.ndarray, min_brightness: typing.SupportsFloat) -> bindings._geom.PolyLine:
    """
    Create a PolyLine geometry
    """
def Sphere(radius: typing.SupportsFloat = 1.0, color: numpy.ndarray = ...) -> bindings._geom.Sphere:
    """
    Create a Sphere geometry
    """
def Triad(scale: typing.SupportsFloat = 1.0, thickness: typing.SupportsFloat = 0.10000000149011612) -> bindings._geom.Triad:
    """
    Create a Triad geometry
    """
