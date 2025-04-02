#pragma once
#include <slam_dunk/node.hpp>

namespace sdunk {
class Tree {
   public:
    Tree();

   private:
    std::unique_ptr<Node> tree_root;
};
}  // namespace sdunk