#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    glm::mat4 getViewMatrix() const;

    // camera controls
    void move(const glm::vec3 position, float deltaTime);
    void rotate(float xoffset, float yoffset);

private:
    // translation
    glm::vec3 getRight() const
    {
        return glm::normalize(glm::cross(forward, worldUp));
    }
    glm::vec3 position{0.0f, 0.0f, 6.0f};
    glm::vec3 forward{0.0f, 0.0f, -1.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

    // rotation
    float yaw = -90.0f;
    float pitch = 0.0f;
    float sensitivity = 0.1f;
};