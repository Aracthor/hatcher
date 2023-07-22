#include "Camera.hpp"

#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/ISaveLoader.hpp"

Camera::Camera()
{
    const float angleX = M_PI / 4.f;
    const float angleY = M_PI / 4.f;

    target = {0.f, 0.f, 0.f};
    position.x = glm::sin(angleY) * glm::cos(angleX);
    position.y = glm::sin(angleY) * glm::sin(angleX);
    position.z = glm::cos(angleY);
    position = position * 100.f;
    up.x = -glm::cos(angleX);
    up.y = -glm::sin(angleX);
    up.z = glm::sin(angleY);
}

glm::vec2 Camera::MouseCoordsToWorldCoords(int x, int y, const IFrameRenderer& frameRenderer) const
{
    const glm::vec3 worldCoords = frameRenderer.WindowCoordsToWorldCoords(glm::vec2(x, y));
    const glm::vec3 cameraToTarget = position - target;
    const float t = worldCoords.z / cameraToTarget.z;
    const glm::vec3 projectedWorldCoords = worldCoords - cameraToTarget * t;
    return static_cast<glm::vec2>(projectedWorldCoords);
}

void Camera::SaveLoad(ISaveLoader& saveLoader)
{
    saveLoader << position;
    saveLoader << target;
    saveLoader << up;
    saveLoader << pixelSize;
}
