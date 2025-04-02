#include <slam_dunk/tree.hpp>

namespace sdunk {

Tree::Tree() {
    this->tree_root = std::make_unique<Node>();
}

}  // namespace sdunk