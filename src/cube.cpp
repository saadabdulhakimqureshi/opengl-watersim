#include "cube.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

namespace
{
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;
    const float cubeSize = 0.3f;
    std::vector<glm::vec3> cubePositions;
}

void initializeCubes()
{
    const float vertices[] = {
        // Face 1
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        // Face 2
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        // Face 3
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Face 4
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        // Face 5
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        // Face 6
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void spawnCube(const glm::vec3& position)
{
    cubePositions.push_back(position);
    //listCubes();
}

void drawCubes(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);
    const GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
    const GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLocation, 0.1f, 0.5f, 0.1f);
    glBindVertexArray(cubeVAO);
    for (const glm::vec3& position : cubePositions)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::scale(model, glm::vec3(cubeSize));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
}

void cleanupCubes()
{
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    cubePositions.clear();
}

void listCubes()
{
    std::cout << "Cube Total: " << cubePositions.size() << '\n';
    for (std::size_t i = 0; i < cubePositions.size(); ++i)
    {
        const glm::vec3& p = cubePositions[i];
        std::cout << "Cube #" << i + 1 << ": (" << p.x << ", " << p.y << ", " << p.z << ")\n";
    }
    std::cout << std::endl;
}