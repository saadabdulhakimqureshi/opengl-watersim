#include "environment.h"
#include "cube.h"

#include <iostream>
#include <memory>
#include <vector>

namespace
{
    std::vector<std::unique_ptr<Object>> objects;
}

void spawnCube(const glm::vec3 &position)
{
    objects.push_back(std::make_unique<Cube>(position));
}

void drawObjects(unsigned int shaderProgram)
{
    for (const auto &object : objects)
    {
        object->drawObject(shaderProgram);
    }
}

void updateObject(float deltaTime)
{
    for (const auto &object : objects)
    {
        object->update(deltaTime);
    }
}

void cleanupObjects()
{
    objects.clear();
}

void listObjects()
{
    std::cout << "Cube Total: " << objects.size() << '\n';

    for (std::size_t i = 0; i < objects.size(); ++i)
    {
        const glm::vec3 &p = objects[i]->getPosition();

        std::cout << "Cube #" << i + 1 << ": ("
                  << p.x << ", "
                  << p.y << ", "
                  << p.z << ")\n";
    }
}