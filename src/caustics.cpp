#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

using namespace std;

// Worley noise generation for faking caustics at the pond floor

// Creates a random, normalized local position within each cell of a square grid of the given size.
vector<vector<glm::vec2>> generateFeatureGrid(int size)
{
    vector<vector<glm::vec2>> featureGrid(size, vector<glm::vec2>(size));

    for (int x = 0; x < size; ++x)
    {
        for (int y = 0; y < size; ++y)
        {
            featureGrid[x][y] = glm::vec2(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
        }
    }

    return featureGrid;
}

// Calculates the normalized distance of the given point to the nearest feature point on the given grid,
// wrapping nonnegative normalized values for seamless tiling.
float worleyNoiseValue(const vector<vector<glm::vec2>> &featureGrid, float normalized_x, float normalized_y)
{
    const int gridWidth = static_cast<int>(featureGrid.size());
    const int gridHeight = static_cast<int>(featureGrid[0].size());

    // Convert normalized coordinates to grid coordinates
    const float x = std::fmod(normalized_x, 1.0f) * gridWidth;
    const float y = std::fmod(normalized_y, 1.0f) * gridHeight;

    // Grid cell containing the sample point
    const int start_x = static_cast<int>(std::floor(x));
    const int start_y = static_cast<int>(std::floor(y));

    float minDist = std::numeric_limits<float>::max();

    // Search the surrounding 3x3 grid cells for the closest feature point
    for (int offset_x = -1; offset_x <= 1; ++offset_x)
    {
        for (int offset_y = -1; offset_y <= 1; ++offset_y)
        {
            const int cell_x = start_x + offset_x;
            const int cell_y = start_y + offset_y;

            // Wrap cell indices across grid boundaries
            const int wrapped_x = (cell_x + gridWidth) % gridWidth;
            const int wrapped_y = (cell_y + gridHeight) % gridHeight;

            // Calculate distance to the feature point
            const glm::vec2 feature = glm::vec2(cell_x, cell_y) + featureGrid[wrapped_x][wrapped_y];
            const float distance = glm::distance(glm::vec2(x, y), feature);

            // Update distance to closest feature point
            minDist = std::min(minDist, distance);
        }
    }

    // Normalize distance to [0, 1]
    return glm::clamp(minDist / glm::sqrt(2.0f), 0.0f, 1.0f);
}

// Creates a square 2D Worley noise texture using feature points from the given grid,
// Size specifies the number of texels per texture dimension, while resolution specifies
// the number of texels per grid cell dimension. The noise repeats when the texture exceeds
// the feature grid's total dimensions in texels, and the resulting texture wraps seamlessly
// if size is an exact multiple of resolution times feature grid size.
vector<float> createWorleyTexture(const vector<vector<glm::vec2>> &featureGrid, int size, int resolution)
{
    if (size <= 0 || resolution <= 0 || featureGrid.empty() || featureGrid[0].empty())
        return {};

    // Allocate texture data
    vector<float> texels(size * size);

    const int gridWidth = resolution * static_cast<int>(featureGrid.size());
    const int gridHeight = resolution * static_cast<int>(featureGrid[0].size());

    // Generate Worley noise for each texel
    for (int y = 0; y < size; ++y)
    {
        const float normalized_y = static_cast<float>(y) / gridHeight;
        for (int x = 0; x < size; ++x)
        {
            const float normalized_x = static_cast<float>(x) / gridWidth;
            texels[(y * size) + x] = worleyNoiseValue(featureGrid, normalized_x, normalized_y);
        }
    }

    return texels;
}