#include "environment.h"
#include "cube.h"

#include <iostream>
#include <vector>

namespace
{
    std::vector<Cube> cubes;
}

void initializeCubes()
{
    Cube::initializeMesh();
}

void spawnCube(const glm::vec3 &position)
{
    cubes.emplace_back(position);
}

void drawCubes(unsigned int shaderProgram)
{
    for (const Cube &cube : cubes)
    {
        cube.drawObject(shaderProgram);
    }
}

void updateCubes(float deltaTime)
{
    for (Cube &cube : cubes)
    {
        cube.update(deltaTime);
    }
}

void cleanupCubes()
{
    cubes.clear();
    Cube::cleanupMesh();
}

void listCubes()
{
    std::cout << "Cube Total: " << cubes.size() << '\n';

    for (std::size_t i = 0; i < cubes.size(); ++i)
    {
        const glm::vec3 &p = cubes[i].getPosition();

        std::cout << "Cube #" << i + 1 << ": ("
                  << p.x << ", "
                  << p.y << ", "
                  << p.z << ")\n";
    }
}