#pragma once
#include <filesystem>

namespace fs = std::filesystem;

namespace slamd {

inline fs::path repo_root() {
    return fs::path(__FILE__)  // this file
        .parent_path()         // include folder / slamd
        .parent_path()         // include_folder
        .parent_path();        // repo root
}

inline fs::path shader_folder() {
    return repo_root() / "shaders";
}

}  // namespace slamd