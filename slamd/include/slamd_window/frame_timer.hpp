#pragma once
#include <chrono>

namespace slamdw {

class FrameTimer {
   public:
    FrameTimer();

    void log_frame();

    float timedelta() const;

   private:
    std::chrono::high_resolution_clock::time_point last_frame;
    std::chrono::duration<float> delta{0.0f};
};

}  // namespace slamd