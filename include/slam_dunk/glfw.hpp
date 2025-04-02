#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

namespace sdunk {
namespace glutils {

GLFWwindow*
make_window(std::string window_name, std::size_t width, std::size_t height);

}
}