import slamd

if __name__ == "__main__":
    window = slamd.Window("Hello world", 1000, 1000)

    scene = slamd.scene()

    scene.set_object("/origin", slamd.geom.Triad())

    window.add_scene("scene", scene)

    window.wait_for_close()
