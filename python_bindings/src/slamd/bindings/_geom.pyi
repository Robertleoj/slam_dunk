from __future__ import annotations
import numpy
__all__ = ['Arrows', 'Box', 'CameraFrustum', 'Geometry', 'Image', 'Mesh', 'PointCloud', 'Points2D', 'PolyLine', 'Sphere', 'Triad']
class Arrows(Geometry):
    pass
class Box(Geometry):
    pass
class CameraFrustum(Geometry):
    pass
class Geometry:
    pass
class Image(Geometry):
    pass
class Mesh(Geometry):
    def update_colors(self, colors: numpy.ndarray) -> None:
        ...
    def update_normals(self, normals: numpy.ndarray) -> None:
        ...
    def update_positions(self, positions: numpy.ndarray, recompute_normals: bool = True) -> None:
        ...
class PointCloud(Geometry):
    def update_colors(self, colors: numpy.ndarray) -> None:
        ...
    def update_positions(self, positions: numpy.ndarray) -> None:
        ...
    def update_radii(self, radii: list[float]) -> None:
        ...
class Points2D(Geometry):
    pass
class PolyLine(Geometry):
    pass
class Sphere(Geometry):
    pass
class Triad(Geometry):
    pass
