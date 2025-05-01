#pragma once

#include <spdlog/spdlog.h>

namespace slamd {

inline void spawn_window() {
    std::string executable_path(EXEC_PATH);
    system(executable_path.c_str());
}

}  // namespace slamd