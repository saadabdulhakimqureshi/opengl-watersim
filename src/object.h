#pragma once

#include <glm/glm.hpp>

class Object
{
public:
    virtual ~Object();

    // GPU resources belong to this object and must not be copied.
    Object(const Object &) = delete;
    Object &operator=(const Object &) = delete;

    void initializeObject(glm::vec3 position);
    void cleanupMesh();

    virtual void drawObject(unsigned int shaderProgram) const;
    virtual void update(float deltaTime, glm::vec3 g);

    const glm::vec3 &getPosition() const
    {
        return position;
    }

    const glm::vec3 &getVelocity() const
    {
        return velocity;
    }

protected:
    Object() = default;

    void initializeMesh(const float *vertices, int vertexCount);

    // transform
    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};

    // visual
    glm::vec3 color{1.0f};

    // rigid body
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    glm::float32 mass{1};

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    int vertexCount = 0;
};