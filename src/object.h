#pragma once

#include <glm/glm.hpp>
#include "boundingRegion.h"

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

    // rigidbody
    virtual void update(float deltaTime, glm::vec3 g);
    float getInverseMass() const;
    void applyImpulse(const glm::vec3 &impulse);

    virtual void applyForce(glm::vec3 force, float deltaTime);
    virtual void applyForce(glm::vec3 direction, float magnitude, float deltaTime);
    virtual void applyAcceleration(glm::vec3 acceleration);
    virtual void applyAcceleration(glm::vec3 direction, float magnitude);
    virtual void transferEnergy(float joulses);

    const glm::vec3 &getPosition() const
    {
        return position;
    }

    const glm::vec3 &getVelocity() const
    {
        return velocity;
    }

    const glm::vec3 &getAcceleration() const
    {
        return acceleration;
    }

    const glm::float32 &getMass() const
    {
        return mass;
    }

    BoundingRegion getBoundingRegion() const;

    void setScale(const glm::vec3 &scale)
    {
        this->scale = scale;
    }

    void setMass(float mass)
    {
        this->mass = mass;
    }

    void setColor(const glm::vec3 &color)
    {
        this->color = color;
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
    glm::vec3 pendingAcceleration{0.0f};
    glm::float32 mass{0.1};

    BoundingRegion localBounds;

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    int vertexCount = 0;
};
