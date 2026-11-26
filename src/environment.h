#pragma once

#include <glm/glm.hpp>

// mesh specific spawning
void spawnCube(const glm::vec3 &position);

// base object class
void drawObjects(unsigned int shaderProgram);
void cleanupObjects();
void listObjects();
void updateObjects(float deltaTime);