#pragma once

#include <glm/glm.hpp>

class BoundingRegion
{
public:
    glm::vec3 min{0.0f};
    glm::vec3 max{0.0f};

    BoundingRegion() = default;

    BoundingRegion(glm::vec3 min, glm::vec3 max)
        : min(min), max(max)
    {
    }

    glm::vec3 calculateCenter() const
    {
        return (min + max) * 0.5f;
    }

    glm::vec3 calculateDimensions() const
    {
        return max - min;
    }

    bool intersectsWith(const BoundingRegion &other) const
    {
        const glm::vec3 halfSize =
            calculateDimensions() * 0.5f;

        const glm::vec3 otherHalfSize =
            other.calculateDimensions() * 0.5f;

        const glm::vec3 distance = glm::abs(
            calculateCenter() - other.calculateCenter());

        for (int axis = 0; axis < 3; ++axis)
        {
            if (distance[axis] > halfSize[axis] + otherHalfSize[axis])
                return false;
        }

        return true;
    }
};