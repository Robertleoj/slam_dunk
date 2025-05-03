#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace slamd {

void spawn_window(
    uint16_t port = 5555,
    std::optional<std::string> exe_path = std::nullopt
);

}  // namespace slamd