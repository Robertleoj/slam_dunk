import slamd
import numpy as np

if __name__ == "__main__":
    window = slamd.Window("two windows", 1000, 1000)

    scene1 = slamd.Scene()
    scene2 = slamd.Scene()

    window.add_scene("scene 1", scene1)
    scene1.set_object("/box", slamd.geom.Box())

    window.add_scene("scene 2", scene2)
    scene2.set_object("/origin", slamd.geom.Triad())

    scene2.set_object("/ball", slamd.geom.Sphere(2.0))

    sphere_transform = np.identity(4, dtype=np.float32)
    sphere_transform[:, 3] = np.array([5.0, 1.0, 2.0, 1.0])

    scene2.set_transform("/ball", sphere_transform)

    window.wait_for_close()
