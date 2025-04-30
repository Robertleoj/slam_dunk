#pragma once
#include <optional>
#include <thread>

namespace slamd {

void assert_thread(std::thread::id thread_id);

template <typename T>
void assert_nil(
    std::optional<T> val
) {
    if (val.has_value()) {
        throw std::runtime_error("Option has value!");
    }
}

}  // namespace slamd