import slamd
import time
import numpy as np


def f(inp, t):
    return np.sin(0.5 * inp[:, :, 0] + t) * np.sin(0.5 * inp[:, :, 1] + t)


def make_grid(n: int, a: float):
    lin = np.linspace(-a, a, n)
    x, y = np.meshgrid(lin, lin)
    coords = np.stack((x, y), axis=-1)
    return coords


def main():
    window = slamd.Window("hello python", 1000, 1000)

    coords = make_grid(30, 10.0)

    scene = slamd.scene()
    window.add_scene("scene", scene)

    t = 0
    while True:
        time.sleep(0.01)
        z = f(coords, t)
        points = np.concatenate((coords, z[:, :, None]), axis=2)

        red = np.exp(-points[:, :, 2])
        blue = 1.0 - red
        green = 0.5

        colors = np.zeros(points.shape, dtype=np.float32)
        colors[:, :, 0] = red
        colors[:, :, 1] = blue
        colors[:, :, 2] = green

        radii = np.ones(points.shape[:2], dtype=float) * 0.3

        scene.set_object(
            "/points",
            slamd.geom.point_cloud(
                points.reshape(-1, 3).astype(np.float32),
                colors.reshape(-1, 3).astype(np.float32),
                radii.reshape(-1).astype(float),
            ),
        )

        t += 0.02

    window.wait_for_close()


if __name__ == "__main__":
    main()
