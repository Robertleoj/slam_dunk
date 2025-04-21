#include <spdlog/spdlog.h>
#include <visualizer_generated.h>
#include <asio.hpp>
#include <iostream>
#include <slamd_window/window.hpp>

int main() {
    spdlog::set_level(spdlog::level::debug);

    slamdw::Window window{};
    window.state_manager.try_connect("127.0.0.1", 5555);

    window.run();
}