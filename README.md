![](./images/logo.png)

---

SlamDunk is a powerful and user-friendly C++/Python library for making live 3D and 2D visualizations for prototyping, data exploration, and algorithm development.

It is very lightweight, built using OpenGL and ImGui.

# Examples

## Hello world

Here is a simple "hello world" program for a SlamDunk visualization.

```python
#python
import slamd

if __name__ == "__main__":
    vis = slamd.Visualizer("Hello world")

    scene = vis.scene("scene")

    scene.set_object("/origin", slamd.geom.Triad())

    vis.hang_forever()
```

```c++
// C++
#include <slamd/slamd.hpp>

int main() {
    auto vis = slamd::visualizer("hello_world");

    auto scene = vis->scene("scene");

    scene->set_object("/origin", slamd::geom::triad());

    vis->hang_forever();
}
```

This example highlights the main components of SlamDunk.

The `Visualizer` object maintains the state of the visualization, and starts a TCP server that the visualization window connects to.

By default, it spawns a window process that reads from it and displays the visualizations. You can opt out of this with the `spawn` argument, and control the port with the `port` argument.
In this case, you can start a window with the `slamd-window` executable:

```
slamd-window --port [port] --ip [ip]
```

If you are using C++, this executable is built next to the library.

This client-server architecture allows launching a visualizer a remote server, and connecting to it on your local machine.

A `Scene` object represents and contains a tree of 3D objects, accessed by paths like `/comp1/comp2/comp3`. 3D poses and `Geometry` objects can be assigned with the `set_transform` and `set_object` methods.

`Geometry` objects represent the objects that are displayed in the scene.

Running this program results in the following interactive visualization:
![](./images/hello_world.png)

## Multiple scenes

SlamDunk uses ImGui to allow multiple sub-windows with floating and docking support inside the SlamDunk viewer. The following example illustrates creating two windows, each showing its own scene.

```python
# python
import slamd
import numpy as np

if __name__ == "__main__":
    vis = slamd.Visualizer("two windows")

    scene1 = vis.scene("scene 1")
    scene2 = vis.scene("scene 2")

    scene1.set_object("/box", slamd.geom.Box())

    scene2.set_object("/origin", slamd.geom.Triad())

    scene2.set_object("/ball", slamd.geom.Sphere(2.0))

    sphere_transform = np.identity(4, dtype=np.float32)
    sphere_transform[:, 3] = np.array([5.0, 1.0, 2.0, 1.0])

    scene2.set_transform("/ball", sphere_transform)

    vis.hang_forever()

```

```c++
// C++
#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    auto vis = slamd::visualizer("two windows");

    auto scene1 = vis->scene("scene 1");
    auto scene2 = vis->scene("scene 2");

    scene1->set_object("/box", slamd::geom::box());

    scene2->set_object("/origin", slamd::geom::triad());
    scene2->set_object("/ball", slamd::geom::sphere(2.0f));

    glm::mat4 sphere_transform(1.0);
    sphere_transform[3] += glm::vec4(5.0, 1.0, 2.0, 1.0);

    scene2->set_transform("/ball", sphere_transform);

    vis->hang_forever();
}

```

The resulting window looks like this:

![](./images/two_scenes.png)

The windows are fully controllable - you can drag then around, make tabs, use them in floating mode, dock them to the sides like you see in the screenshot. All of this is supported by [ImGui](https://github.com/ocornut/imgui).

## Further reading

The C++ examples in `/examples` and python examples in `python_examples` showcase some more features of SlamDunk. Some examples are canvases for 2D visualizations and lots of additional geometry primitives such as point clouds, meshes, camera frustums, etc.

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

All contributions and feedback are welcome and apprechiated!
