![](./images/logo.png)

---

SlamDunk is a simple and user-friendly C++ library for making live 3D and 2D visualizations for prototyping, data exploration, and algorithm development.

It is very lightweight, built using OpenGL and ImGui.

The project is in very early development with many improvements coming in the near future.

# Examples

## Hello world

Here is a simple "hello world" program for a SlamDunk visualization.

```c++
#include <slamd/slamd.hpp>

int main() {
    slamd::Window window(1000, 1000);

    auto scene = slamd::scene();

    scene->set_object("/origin", slamd::geometry::triad());

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

```c++
#include <glm/glm.hpp>
#include <slamd/slamd.hpp>

int main() {
    slamd::Window window(1000, 1000);

    auto scene1 = slamd::scene();
    auto scene2 = slamd::scene();

    window.add_scene("scene 1", scene1);
    scene1->set_object("/box", slamd::geometry::box());

    window.add_scene("scene 2", scene2);
    scene2->set_object("/origin", slamd::geometry::triad());
    scene2->set_object("/ball", slamd::geometry::sphere(2.0f));

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

If you are not using a package manager, the SlamDunk requires the following dependencies available in your build system:

- [`spdlog`](https://github.com/gabime/spdlog)
- [`glbinding`](https://github.com/cginternals/glbinding)
- [`glfw3`](https://github.com/glfw/glfw)
- [`glm`](https://github.com/g-truc/glm)

## With FetchContent

You can use CMake's `FetchContent`. Add this to your `CMakeLists.txt`:

```c++
include(FetchContent)

FetchContent_Declare(
  MyCoolLib
  GIT_REPOSITORY https://github.com/you/yourproject.git
  GIT_TAG main
  SOURCE_SUBDIR mylib
)

FetchContent_MakeAvailable(MyCoolLib)
```

Linking to it then looks like:

```cmake
target_link_libraries(
    your_target PRIVATE

    slamd::slamd
)
```

## With git submodules

If you add the repo as a submodule in your project, you can add it as a subdirectory with

```cmake
add_subdirectory(path/to/slam_dunk/slamd)

```

You can then link it to your executable or library with

```cmake
target_link_libraries(
    your_target PRIVATE

    slamd::slamd
)
```

## With `vcpkg`

We are currently working on a `vcpkg` port for SlamDunk, stay tuned!

# Contributions

All contributions are welcome! SlamDunk is in very early development, so there is enough work to do, and multiple things to improve.
