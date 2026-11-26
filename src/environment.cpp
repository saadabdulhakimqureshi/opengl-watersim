#include "environment.h"

#include <iostream>
#include <memory>
#include <vector>

namespace
{
    std::vector<std::unique_ptr<Object>> objects;

    glm::vec3 gravity{0.0f, -9.81f, 0.0f};
}

Cube *spawnCube(const glm::vec3 &position)
{
    auto cube = std::make_unique<Cube>(position);
    Cube *result = cube.get();

    objects.push_back(std::move(cube));

    return result;
}

void drawObjects(unsigned int shaderProgram)
{
    for (const auto &object : objects)
    {
        object->drawObject(shaderProgram);
    }
}

void updateObjects(float deltaTime)
{
    for (const auto &object : objects)
    {
        object->update(deltaTime, gravity);
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

void applyForceToObject(Object &object, glm::vec3 force, float magnitude, float deltaTime)
{
    object.applyForce(force, magnitude, deltaTime);
}