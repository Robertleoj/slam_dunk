#pragma once
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>

namespace sdunk {
class Node {
   public:
    std::map<std::string, std::unique_ptr<Node>> children;
    std::optional<glm::mat4> pose;
};

class OwningNode : public Node {};

class WeakNode : public Node {};

}  // namespace sdunk