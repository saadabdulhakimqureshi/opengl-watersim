#include <camera.h>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + forward, worldUp);
}