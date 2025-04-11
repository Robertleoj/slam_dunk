import slamd
import time
import numpy as np


def f(inp, t):
    return np.sin(inp[:, :, 0] + t) * np.sin(inp[:, :, 1] + t)


def make_grid(n: int, a: float):
    lin = np.linspace(-a, a, n)
    x, y = np.meshgrid(lin, lin)
    coords = np.stack((x, y), axis=-1)
    return coords


def main():
    window = slamd.Window("hello python", 1000, 1000)

    coords = make_grid(20, 5.0)

    scene = slamd.scene()
    window.add_scene("scene", scene)

    t = 0
    while True:
        time.sleep(0.01)
        z = f(coords, t)
        points = np.concatenate((coords, z[:, :, None]), axis=2)

        scene.set_object(
            "/points",
            slamd.geom.point_cloud(
                points.reshape(-1, 3).astype(np.float32),
                np.array([1.0, 0.0, 0.0], dtype=np.float32),
                0.3,
            ),
        )

        t += 0.1

    window.wait_for_close()


if __name__ == "__main__":
    main()
