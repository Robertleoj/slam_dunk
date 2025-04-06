#pragma once
#include <filesystem>

namespace fs = std::filesystem;

namespace sdunk {

fs::path repo_root() {
    return fs::path(__FILE__)  // this file
        .parent_path()         // include folder / slam_dunk
        .parent_path()         // include_folder
        .parent_path();        // repo root
}

fs::path shader_folder() {
    return repo_root() / "shaders";
}

}  // namespace sdunk