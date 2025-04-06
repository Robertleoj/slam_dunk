#include <slam_dunk/tree_path.hpp>
#include <stdexcept>
#include <vector>

namespace sdunk {

std::vector<std::string> split_path(
    const std::string& path
) {
    if (path.empty() || path[0] != '/') {
        throw std::invalid_argument("Path must start with '/'");
    }

    std::vector<std::string> components;
    size_t start = 1, end;

    while ((end = path.find('/', start)) != std::string::npos) {
        if (end == start) {
            throw std::invalid_argument("Empty component in path");
        }
        components.push_back(path.substr(start, end - start));
        start = end + 1;
    }

    if (start >= path.size()) {
        throw std::invalid_argument("Path must not end with '/'");
    }

    std::string last = path.substr(start);
    if (last.empty()) {
        throw std::invalid_argument("Empty component in path");
    }

    components.push_back(last);
    return components;
}

TreePath::TreePath(
    const std::string& path_string
) {
    this->components = split_path(path_string);
}

}  // namespace sdunk