#pragma once

#include <glm/glm.hpp>


namespace sdunk {

class Arcball {

public:
    glm::mat4 center;
    glm::mat4 camera_in_center;
};

}