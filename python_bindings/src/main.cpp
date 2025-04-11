#include <pybind11/pybind11.h>
#include <slamd/slamd.hpp>

namespace py = pybind11;

int add_numbers(
    int a,
    int b
) {
    return a + b;
}

int basic_window() {
    slamd::Window window("hello python", 1000, 1000);

    auto scene = slamd::scene();

    window.add_scene("scene", scene);

    window.wait_for_close();
}

PYBIND11_MODULE(
    _core,
    m
) {
    m.doc() = "SlamDunk visualization library";

    m.def("add", &add_numbers, "Add two numbers");
    m.def("hello_python", &basic_window, "SlamDunk window");
}
