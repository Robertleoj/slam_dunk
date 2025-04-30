#include <flatb/visualizer_generated.h>
#include <spdlog/spdlog.h>
#include <asio.hpp>
#include <iostream>
#include <slamd_window/window.hpp>

int main() {
    spdlog::set_level(spdlog::level::debug);

    slamd::Window window{};
    window.state_manager.try_connect("127.0.0.1", 5555);

    window.run();
}