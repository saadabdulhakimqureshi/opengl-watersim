#include <glm/glm.hpp>

// Horizontal square mesh generation for testing water surfaces at different resolutions

// Generates vertices and UV coordinates for a square mesh of triangles in the x-z plane.
// Parameters specify the mesh size, number of squares (two triangles) per dimension, and y-coordinate.
// Stores the generated values in the provided arrays.
void generateSquareMesh(float size, int resolution, float height, glm::vec3 *positions, glm::vec2 *uvs)
{
    const float halfSize = size * 0.5f;       // Half the grid
    const float cellSize = size / resolution; // A square dimension

    int idx = 0;
    for (int z = 0; z < resolution; ++z)
    {
        for (int x = 0; x < resolution; ++x)
        {
            // Positions of square corners
            const float x0 = x * cellSize - halfSize;
            const float z0 = z * cellSize - halfSize;
            const float x1 = x0 + cellSize;
            const float z1 = z0 + cellSize;

            const glm::vec3 p00(x0, height, z0);
            const glm::vec3 p10(x1, height, z0);
            const glm::vec3 p01(x0, height, z1);
            const glm::vec3 p11(x1, height, z1);

            // Texture coordinates of square corners
            const float u0 = static_cast<float>(x) / resolution;
            const float u1 = static_cast<float>(x + 1) / resolution;
            const float v0 = static_cast<float>(z) / resolution;
            const float v1 = static_cast<float>(z + 1) / resolution;

            const glm::vec2 uv00(u0, v0);
            const glm::vec2 uv10(u1, v0);
            const glm::vec2 uv01(u0, v1);
            const glm::vec2 uv11(u1, v1);

            // Triangle 1
            positions[idx] = p00;
            uvs[idx++] = uv00;
            positions[idx] = p10;
            uvs[idx++] = uv10;
            positions[idx] = p11;
            uvs[idx++] = uv11;

            // Triangle 2
            positions[idx] = p11;
            uvs[idx++] = uv11;
            positions[idx] = p01;
            uvs[idx++] = uv01;
            positions[idx] = p00;
            uvs[idx++] = uv00;
        }
    }
}
