#pragma once
#include <string>
#include <vector>

namespace slamd {

class TreePath {
   public:
    TreePath(const std::string& path_string);
    TreePath(const std::vector<std::string>& components);
    TreePath(const char* path_string);
    bool is_root() const;
    TreePath parent() const;
    std::string string() const;

   public:
    const std::vector<std::string> components;
};

TreePath operator/(const TreePath& path, const std::string& part);

}  // namespace slamd