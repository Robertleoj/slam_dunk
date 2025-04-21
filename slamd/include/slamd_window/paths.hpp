#pragma once
#include <filesystem>

namespace fs = std::filesystem;

namespace slamdw {

inline fs::path repo_root() {
    return fs::path(__FILE__)  // this file
        .parent_path()         // include folder / slamd
        .parent_path()         // include_folder
        .parent_path();        // repo root
}

}  // namespace slamd