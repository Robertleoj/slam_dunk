#include <spdlog/spdlog.h>
#include <chrono>
#include <slamd/visualizer.hpp>
#include <thread>

int main() {
    spdlog::set_level(spdlog::level::debug);

    slamd::Visualizer vis("hello viz");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}