#include "cube.h"

Cube::Cube(const glm::vec3 &position)
{
    initializeObject(position);

    scale = glm::vec3(0.3f);
    color = glm::vec3(0.1f, 0.5f, 0.1f);
    localBounds = BoundingRegion(
        glm::vec3(-0.5f),
        glm::vec3(0.5f));

    constexpr int vertexCount =
        static_cast<int>(sizeof(vertices) / sizeof(vertices[0]) / 3);

    initializeMesh(vertices, vertexCount);
}