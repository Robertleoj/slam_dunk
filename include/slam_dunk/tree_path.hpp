#pragma once
#include <string>
#include <vector>

namespace sdunk {

class TreePath {
   public:
    TreePath(const std::string& path_string);
    TreePath(const std::vector<std::string>& components);
    TreePath(const char* path_string);

    const std::vector<std::string> components;

    bool is_root() const;

    TreePath parent() const;
};

TreePath operator/(const TreePath& path, const std::string& part);
}  // namespace sdunk