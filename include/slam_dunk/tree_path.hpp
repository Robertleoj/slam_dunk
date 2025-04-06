#pragma once
#include <string>
#include <vector>

namespace sdunk {

class TreePath {
   public:
    TreePath(const std::string& path_string);
    std::vector<std::string> components;
};

}  // namespace sdunk