#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    glm::mat4 getViewMatrix() const;

private:
    glm::vec3 position{0.0f, 0.0f, 6.0f};
    glm::vec3 forward{0.0f, 0.0f, -1.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
};