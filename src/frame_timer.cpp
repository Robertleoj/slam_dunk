#include <slam_dunk/frame_timer.hpp>

namespace sdunk {

FrameTimer::FrameTimer()
    : last_frame(std::chrono::high_resolution_clock::now()) {}

void FrameTimer::log_frame() {
    std::chrono::high_resolution_clock::time_point now =
        std::chrono::high_resolution_clock::now();
    delta = now - last_frame;
    last_frame = now;
}

float FrameTimer::timedelta() const {
    return delta.count();
}

}  // namespace sdunk