#include <slam_dunk/tree_path.hpp>
#include <stdexcept>
#include <vector>

namespace sdunk {

std::vector<std::string> split_path(
    const std::string& path
) {
    // allow the root path
    if (path == "/") {
        return {};
    }

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
)
    : components(split_path(path_string)) {}

TreePath::TreePath(
    const std::vector<std::string>& components
)
    : components(components) {}

bool TreePath::is_root() const {
    return this->components.size() == 0;
}

TreePath TreePath::parent() const {
    if (this->components.size() == 0) {
        throw std::runtime_error("Cannot pop empty path");
    }

    return TreePath(
        std::vector(this->components.begin(), this->components.end() - 1)
    );
}

TreePath operator/(
    const TreePath& path,
    const std::string& part
) {
    std::vector<std::string> new_components = path.components;
    new_components.push_back(part);
    return TreePath(new_components);
}

}  // namespace sdunk
