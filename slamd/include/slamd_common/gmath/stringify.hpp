#pragma once

#include <glm/glm.hpp>
#include <iomanip>
#include <sstream>
#include <string>

namespace slamd {
namespace gmath {

inline std::string stringify(
    const glm::mat4& mat
) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);  // 3 decimals, clean bro

    for (int row = 0; row < 4; ++row) {
        oss << "| ";
        for (int col = 0; col < 4; ++col) {
            oss << std::setw(8) << mat[col][row]
                << " ";  // glm is column-major bro
        }
        oss << "|\n";
    }
    return oss.str();
}

}  // namespace gmath
}  // namespace slamd