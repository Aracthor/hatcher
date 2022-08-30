#include "Camera.hpp"

#include "hatcher/Graphics/IFrameRenderer.hpp"

glm::vec2 Camera::MouseCoordsToWorldCoords(int x, int y, const IFrameRenderer& frameRenderer) const
{
    const glm::vec3 worldCoords = frameRenderer.WindowCoordsToWorldCoords(glm::vec2(x, y));
    const glm::vec3 cameraToTarget = position - target;
    const float t = worldCoords.z / cameraToTarget.z;
    const glm::vec3 projectedWorldCoords = worldCoords - cameraToTarget * t;
    return static_cast<glm::vec2>(projectedWorldCoords);
}
