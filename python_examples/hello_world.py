import slamd

if __name__ == "__main__":
    vis = slamd.Visualizer("Hello world")

    scene = slamd.Scene()

    scene.set_object("/origin", slamd.geom.Triad())

    vis.add_scene("scene", scene)

    vis.hang_forever()
