from __future__ import annotations
import numpy
import slamd.bindings._geom
import typing
__all__ = ['arrows', 'box', 'camera_frustum', 'mono_mesh', 'point_cloud', 'poly_line', 'simple_mesh', 'sphere', 'triad']
def arrows(starts: numpy.ndarray[float32[n][3]], ends: numpy.ndarray[float32[n][3]], colors: numpy.ndarray[float32[n][3]], thickness: float) -> slamd.bindings._geom.Arrows:
    """
    Create an Arrows geometry
    """
def box() -> slamd.bindings._geom.Box:
    """
    Create a Box geometry
    """
def camera_frustum(*args, **kwargs) -> slamd.bindings._geom.CameraFrustum:
    """
    Create a CameraFrustum geometry
    """
def mono_mesh(vertices: numpy.ndarray[float32[n][3]], triangle_indices: list[int], color: numpy.ndarray[float32[3]]) -> slamd.bindings._geom.MonoMesh:
    """
    Create a MonoMesh geometry
    """
@typing.overload
def point_cloud(positions: numpy.ndarray[float32[n][3]], color: numpy.ndarray[float32[3]], radius: float) -> slamd.bindings._geom.PointCloud:
    """
    Create a PointCloud with uniform color and radius
    """
@typing.overload
def point_cloud(positions: numpy.ndarray[float32[n][3]], colors: numpy.ndarray[float32[n][3]], radii: list[float]) -> slamd.bindings._geom.PointCloud:
    """
    Create a PointCloud with per-point color and radius
    """
def poly_line(points: numpy.ndarray[float32[n][3]], thickness: float, color: numpy.ndarray[float32[3]]) -> slamd.bindings._geom.PolyLine:
    """
    Create a PolyLine geometry
    """
def simple_mesh(vertices: numpy.ndarray[float32[n][3]], vertex_colors: numpy.ndarray[float32[n][3]], triangle_indices: list[int]) -> slamd.bindings._geom.SimpleMesh:
    """
    Create a SimpleMesh geometry from raw data
    """
def sphere(radius: float = 1.0, color: numpy.ndarray[float32[3]] = ...) -> slamd.bindings._geom.Sphere:
    """
    Create a Sphere geometry
    """
def triad(scale: float = 1.0, thickness: float = 0.10000000149011612) -> slamd.bindings._geom.Triad:
    """
    Create a Triad geometry
    """
