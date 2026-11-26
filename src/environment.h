#pragma once

#include <glm/glm.hpp>
#include "object.h"
#include "cube.h"

// mesh specific spawning
Cube *spawnCube(const glm::vec3 &position);

// base object class
void drawObjects(unsigned int shaderProgram);
void cleanupObjects();
void listObjects();
void updateObjects(float deltaTime);
void applyForceToObject(Object &object, glm::vec3 force, float magnitude, float deltaTime);