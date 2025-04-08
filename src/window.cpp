#include <slamd/window.hpp>

namespace slamd {

Window::Window(
    size_t height,
    size_t width
) {
    this->render_thread = std::thread(&Window::render_job, this, height, width);
}

void Window::render_job(
    size_t height,
    size_t width
) {
    this->window = slamd::glutils::make_window("Slam Dunk", width, height);
}

}  // namespace slamd