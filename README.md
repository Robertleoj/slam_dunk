![](./images/logo.png)

---

SlamDunk is a powerful and user-friendly C++/Python library for making live 3D and 2D visualizations for prototyping, data exploration, and algorithm development.

It is very lightweight, built using OpenGL and ImGui.

The project is in very early development with many improvements coming in the near future.

# Examples

## Hello world

Here is a simple "hello world" program for a SlamDunk visualization.

```python
# python
import slamd

if __name__ == "__main__":
    window = slamd.Window("Hello world", 1000, 1000)

    scene = slamd.Scene()

    scene.set_object("/origin", slamd.geom.Triad())

    window.add_scene("scene", scene)

    window.wait_for_close()
```

```c++
// C++
#include <slamd/slamd.hpp>

int main() {
    slamd::Window window("Hello world", 1000, 1000);

    auto scene = slamd::scene();

    scene->set_object("/origin", slamd::geom::triad());

    window.add_scene("scene", scene);

    window.wait_for_close();
}
```

This example highlights the main components of SlamDunk.

1. The `Window` object handles the actual display window.
2. A `Scene` object represents and contains a tree of 3D objects.
3. `Geometry` objects can be added to `Scene`s with a path in the tree.
4. To display a scene, we must add it to the window - this creates a _view_ of the scene.

Running this program results in the following interactive visualization:
![](./images/hello_world.png)

## Multiple scenes

We use ImGui to allow multiple sub-windows with floating and docking support inside the SlamDunk viewer. The following example illustrates creating two windows, each showing its own scene.

```python
# python
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

```

```c++
// C++
#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    slamd::Window window("two windows", 1000, 1000);

    auto scene1 = slamd::scene();
    auto scene2 = slamd::scene();

    window.add_scene("scene 1", scene1);
    scene1->set_object("/box", slamd::geom::box());

    window.add_scene("scene 2", scene2);
    scene2->set_object("/origin", slamd::geom::triad());
    scene2->set_object("/ball", slamd::geom::sphere(2.0f));

    glm::mat4 sphere_transform(1.0);
    sphere_transform[3] += glm::vec4(5.0, 1.0, 2.0, 1.0);

    scene2->set_transform("/ball", sphere_transform);

    window.wait_for_close();
}

```

The resulting window looks like this:

![](./images/two_scenes.png)

The windows are fully controllable - you can drag then around, make tabs, use them in floating mode, dock them to the sides like you see in the screenshot. All of this is supported by [ImGui](https://github.com/ocornut/imgui).

## Further reading

The examples in the `/examples` folder showcase some more features of SlamDunk, like

- Canvases for 2D visualizations
- Multiple views of the same scene
- Moving objects around.
- Taking control of the render loop to create fully-fletced GUIs around SlamDunk using the power of ImGui.

# Installation

## Python

The python binding wheels are available on [PyPi](https://pypi.org/project/slamd/), so you can simply

```bash
pip install slamd
```

## C++

### With FetchContent

You can use CMake's `FetchContent`. Add this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  slamd
  GIT_REPOSITORY https://github.com/Robertleoj/slam_dunk.git
  GIT_TAG main
  SOURCE_SUBDIR slamd
)

FetchContent_MakeAvailable(slamd)
```

Linking to it then looks like:

```cmake
target_link_libraries(
    your_target PRIVATE

    slamd::slamd
)
```

### With git submodules

If you add the repo as a submodule in your project, you can add it as a subdirectory with

```cmake
add_subdirectory(path/to/slam_dunk/slamd)

```

Just make sure to

```bash
git submodule update --init --recursive

```

inside the submodule, as all necessary dependencies are vendored.

You can then link it to your executable or library with

```cmake
target_link_libraries(
    your_target PRIVATE

    slamd::slamd
)
```

# Contributions

All contributions are welcome! SlamDunk is in very early development, so there is enough work to do, and multiple things to improve.
