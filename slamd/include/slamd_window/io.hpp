#pragma once
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace slamdw {

std::string read_file_text(const fs::path& path);

}  // namespace slamd