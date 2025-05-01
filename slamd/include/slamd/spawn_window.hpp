#pragma once

#include <spawn.h>
#include <spdlog/spdlog.h>

namespace slamd {

inline void spawn_window() {
    std::string executable_path(EXEC_PATH);
    pid_t pid;
    const char* path = EXEC_PATH;
    char* const argv[] = {(char*)"slamd_window", NULL};

    posix_spawn(&pid, path, NULL, NULL, argv, environ);
}

}  // namespace slamd