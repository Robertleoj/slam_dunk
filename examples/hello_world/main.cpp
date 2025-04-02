#include <chrono>
#include <slam_dunk/renderer.hpp>
#include <slam_dunk/slam_dunk.hpp>
#include <thread>

int main() {
    sdunk::Renderer renderer{};

    std::this_thread::sleep_for(std::chrono::seconds(2));
}