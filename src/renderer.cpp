#include <chrono>
#include <slam_dunk/renderer.hpp>

namespace sdunk {

Renderer::Renderer() {
    render_thread = std::thread(&Renderer::render_job, this);
}

void Renderer::render_job() {
    while (!this->should_stop) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

Renderer::~Renderer() {
    this->should_stop = true;
    if (this->render_thread.joinable()) {
        this->render_thread.join();
    }
}

}  // namespace sdunk