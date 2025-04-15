import slamd
import numpy as np


def make_circles(n: int = 100):
    positions = np.random.uniform(-1000.0, 2000.0, (n, 2))
    colors = np.random.uniform(0.0, 1.0, (n, 3))
    radii = np.random.uniform(1.0, 10.0, n)
    thickness = 0.1

    return slamd.geom2d.Circles(positions, colors, radii, thickness)


def main():
    window = slamd.Window("image", 1000, 1000)

    canvas = slamd.Canvas()

    circles = make_circles(100000)

    canvas.set_object("/circles", circles)

    window.add_canvas("canvas", canvas)

    window.wait_for_close()


if __name__ == "__main__":
    main()
