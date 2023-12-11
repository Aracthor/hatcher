#include "Camera.hpp"

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ComponentSaver.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"

glm::vec2 Camera::MouseCoordsToWorldCoords(int x, int y, const IFrameRenderer& frameRenderer) const
{
    const glm::vec3 worldCoords = frameRenderer.WindowCoordsToWorldCoords(glm::vec2(x, y));
    const glm::vec3 cameraToTarget = Position() - Target();
    const float t = worldCoords.z / cameraToTarget.z;
    const glm::vec3 projectedWorldCoords = worldCoords - cameraToTarget * t;
    return static_cast<glm::vec2>(projectedWorldCoords);
}

glm::vec3 Camera::Position() const
{
    glm::vec3 sphericalCoordinates;
    sphericalCoordinates.x = glm::sin(angles.y) * glm::cos(angles.x);
    sphericalCoordinates.y = glm::sin(angles.y) * glm::sin(angles.x);
    sphericalCoordinates.z = glm::cos(angles.y);
    return sphericalCoordinates * 100.f + target;
}

glm::vec3 Camera::Target() const
{
    return target;
}

glm::vec3 Camera::Up() const
{
    glm::vec3 up;
    up.x = -glm::cos(angles.x);
    up.y = -glm::sin(angles.x);
    up.z = glm::sin(angles.y);
    return up;
}

void Camera::Save(ComponentSaver& saver) const
{
    saver << target;
    saver << angles;
    saver << pixelSize;
}

void Camera::Load(ComponentLoader& loader)
{
    loader >> target;
    loader >> angles;
    loader >> pixelSize;
}

namespace
{
WorldComponentTypeRegisterer<Camera, EComponentList::Rendering> registerer;
} // namespace
