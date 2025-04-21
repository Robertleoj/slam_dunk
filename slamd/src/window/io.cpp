#include <format>
#include <fstream>
#include <slamd_window/io.hpp>

namespace slamdw {
std::string read_file_text(
    const fs::path& path
) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error(
            std::format("Couldn't open {}", path.string())
        );
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}
}  // namespace slamd