from __future__ import annotations
import numpy
__all__ = ['Arrows', 'Box', 'CameraFrustum', 'Geometry', 'Image', 'MonoMesh', 'PointCloud', 'Points2D', 'PolyLine', 'SimpleMesh', 'Sphere', 'Triad']
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
class MonoMesh(Geometry):
    pass
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
class SimpleMesh(Geometry):
    pass
class Sphere(Geometry):
    pass
class Triad(Geometry):
    pass
