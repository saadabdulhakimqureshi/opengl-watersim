#include "object.h"

#include <cmath>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::~Object()
{
    cleanupMesh();
}

void Object::initializeObject(glm::vec3 position)
{
    this->position = position;
}

void Object::initializeMesh(const float *vertices, int vertexCount)
{
    cleanupMesh();

    this->vertexCount = vertexCount;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertexCount * 3 * sizeof(float),
        vertices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::drawObject(unsigned int shaderProgram) const
{
    glUseProgram(shaderProgram);

    const GLint modelLocation =
        glGetUniformLocation(shaderProgram, "model");

    const GLint colorLocation =
        glGetUniformLocation(shaderProgram, "objectColor");

    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), position);

    model = glm::scale(model, scale);

    glUniformMatrix4fv(
        modelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    glUniform3fv(
        colorLocation,
        1,
        glm::value_ptr(color));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

float Object::getInverseMass() const
{
    // Zero or negative mass represents an immovable object.
    return mass > 0.0f ? 1.0f / mass : 0.0f;
}

void Object::update(float deltaTime, glm::vec3 g)
{
    if (getInverseMass() == 0.0f)
    {
        acceleration = glm::vec3(0.0f);
        pendingAcceleration = glm::vec3(0.0f);
        return;
    }

    acceleration = g + pendingAcceleration;

    position += velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;

    velocity += acceleration * deltaTime;

    // Additional acceleration is consumed by this physics step.
    pendingAcceleration = glm::vec3(0.0f);
}

void Object::applyImpulse(const glm::vec3 &impulse)
{
    velocity += impulse * getInverseMass();
}

void Object::applyForce(glm::vec3 force, float deltaTime)
{
    // Impulse = force multiplied by its application duration.
    applyImpulse(force * deltaTime);
}

void Object::applyForce(
    glm::vec3 direction,
    float magnitude,
    float deltaTime)
{
    applyForce(direction * magnitude, deltaTime);
}

void Object::applyAcceleration(glm::vec3 acceleration)
{
    pendingAcceleration += acceleration;
}

void Object::applyAcceleration(
    glm::vec3 direction,
    float magnitude)
{
    applyAcceleration(direction * magnitude);
}

void Object::transferEnergy(float joules)
{
    if (getInverseMass() == 0.0f)
        return;

    const float speedSquared = glm::dot(velocity, velocity);

    if (speedSquared <= 0.0f)
        return;

    const float newSpeedSquared = glm::max(
        0.0f,
        speedSquared + 2.0f * joules / mass);

    velocity *= std::sqrt(newSpeedSquared / speedSquared);
}

BoundingRegion Object::getBoundingRegion() const
{
    const glm::vec3 first =
        localBounds.min * scale + position;

    const glm::vec3 second =
        localBounds.max * scale + position;

    return BoundingRegion(
        glm::min(first, second),
        glm::max(first, second));
}

void Object::cleanupMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    VAO = 0;
    VBO = 0;
    vertexCount = 0;
}
