#include "object.h"

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
        0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), nullptr);

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

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, scale);

    glUniformMatrix4fv(
        modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3fv(colorLocation, 1, glm::value_ptr(color));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void Object::update(float deltaTime, glm::vec3 g)
{
    acceleration = mass * g * deltaTime;
    position += velocity * deltaTime - acceleration * deltaTime;
    velocity -= acceleration;
}

void Object::cleanupMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    VAO = 0;
    VBO = 0;
    vertexCount = 0;
}