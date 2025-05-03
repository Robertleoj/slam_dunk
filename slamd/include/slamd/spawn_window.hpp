#pragma once

#include <spawn.h>
#include <spdlog/spdlog.h>

extern char** environ;
namespace slamd {

inline void spawn_window(
    std::string window_name
) {
    std::string executable_path(EXEC_PATH);
    pid_t pid;
    const char* path = EXEC_PATH;

    std::string window_arg = std::format("-w={}", window_name);

    char* const argv[] = {
        (char*)"slamd_window",
        (char*)window_arg.c_str(),
        NULL
    };

    posix_spawn(&pid, path, NULL, NULL, argv, environ);
}

}  // namespace slamd