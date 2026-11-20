#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + forward, worldUp);
}

// https://learnopengl.com/Getting-started/Camera -> Walking Around + Movement Speed
void Camera::move(const glm::vec3 position, float deltaTime)
{
    const float cameraSpeed = 0.5f; // adjust accordingly

    const glm::vec3 direction =
        getRight() * position.x +
        worldUp * position.y +
        forward * position.z;

    this->position += direction * cameraSpeed * deltaTime;
}

// https://learnopengl.com/Getting-started/Camera -> Look Arround
void Camera::rotate(float xoffset, float yoffset)
{
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;

    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;

    direction.x =
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

    direction.y =
        glm::sin(glm::radians(pitch));

    direction.z =
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

    forward = glm::normalize(direction);
}