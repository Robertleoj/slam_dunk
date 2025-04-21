#pragma once
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace slamd {

std::string read_file_text(const fs::path& path);

}  // namespace slamd