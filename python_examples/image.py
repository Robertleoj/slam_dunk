import slamd
import imageio.v3 as iio
from pathlib import Path


def main():
    window = slamd.Window("image", 1000, 1000)

    canvas = slamd.Canvas()

    image_path = Path(__file__).parent.parent / "images" / "logo.png"

    image = iio.imread(image_path)

    canvas.set_object("/image", slamd.geom2d.Image(image))

    window.add_canvas("canvas", canvas)

    window.wait_for_close()


if __name__ == "__main__":
    main()
