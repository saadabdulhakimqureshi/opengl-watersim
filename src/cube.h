#pragma once

#include "object.h"

class Cube : public Object
{
public:
    explicit Cube(const glm::vec3 &position);

    static void initializeMesh();
    static void cleanupMesh();

    void drawObject(unsigned int shaderProgram) const override;
    void update(float deltaTime);

private:
    static unsigned int cubeVAO;
    static unsigned int cubeVBO;

    float cubeSize = 0.3f;
    glm::vec3 color{0.1f, 0.5f, 0.1f};
};