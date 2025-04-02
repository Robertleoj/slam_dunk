#include <chrono>
#include <slam_dunk/slam_dunk.hpp>
#include <thread>

int main() {
    sdunk::Slamvas slamvas{};

    std::this_thread::sleep_for(std::chrono::seconds(2));
}