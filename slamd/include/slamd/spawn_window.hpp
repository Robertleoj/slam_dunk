#pragma once

#include <filesystem>
#include <optional>

namespace slamd {

void spawn_window(
    std::string window_name,
    std::optional<std::string> exe_path = std::nullopt
);

}  // namespace slamd