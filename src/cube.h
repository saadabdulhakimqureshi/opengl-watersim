#pragma once

#include <glm/glm.hpp>

void initializeCubes();
void spawnCube(const glm::vec3& position);
void drawCubes(unsigned int shaderProgram);
void cleanupCubes();
void listCubes();