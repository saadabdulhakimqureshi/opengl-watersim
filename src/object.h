#pragma once

#include <glm/glm.hpp>

class Object
{
public:
    virtual ~Object() = default;

    void initializeObject(glm::vec3 position);

    const glm::vec3 &getPosition() const
    {
        return position;
    }

    virtual void drawObject(unsigned int shaderProgram) const = 0;

protected:
    glm::vec3 position{0.0f};
};