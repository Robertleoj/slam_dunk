#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace slamd {

void spawn_window(
    std::string window_name,
    uint16_t port = 5555,
    std::optional<std::string> exe_path = std::nullopt
);

}  // namespace slamd