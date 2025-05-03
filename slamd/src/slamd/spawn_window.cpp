#include <spawn.h>
#include <spdlog/spdlog.h>
#include <format>
#include <iostream>
#include <slamd/spawn_window.hpp>

extern char** environ;
namespace slamd {

void spawn_window(
    std::string window_name,
    std::optional<std::string> exe_path
) {
    std::filesystem::path executable_path(exe_path.value_or(EXEC_PATH));
    pid_t pid;

    std::cout << "Executable path: " << executable_path << std::endl;

    if (!std::filesystem::exists(executable_path)) {
        throw std::runtime_error(
            std::format("Executable {} not found", executable_path.string())
        );
    }

    std::string window_arg = std::format("-w={}", window_name);

    char* const argv[] = {
        (char*)"slamd_window",
        (char*)window_arg.c_str(),
        NULL
    };

    posix_spawn(&pid, executable_path.c_str(), NULL, NULL, argv, environ);
}

}  // namespace slamd