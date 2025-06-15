#pragma once

#include "hatcher/Maths/Box.hpp"
#include "hatcher/Maths/glm_pure.hpp"

namespace hatcher
{

class Clock;
class Mesh;

class IFrameRenderer
{
public:
    virtual ~IFrameRenderer() = default;

    virtual void DrawSceneMesh(const Mesh* mesh, const glm::mat4& modelMatrix) = 0;
    virtual void DrawUIMesh(const Mesh* mesh, const glm::mat4& modelMatrix) = 0;
    virtual void SetProjectionMatrix(const glm::mat4& matrix) = 0;
    virtual void SetViewMatrix(const glm::mat4& matrix) = 0;

    virtual const Clock* GetClock() const = 0;
    virtual glm::ivec2 Resolution() const = 0;
    virtual glm::vec2 WorldCoordsToWindowCoords(const glm::vec3& worldCoords, const glm::mat4& modelMatrix) const = 0;
    virtual Box2f ProjectBox3DToWindowCoords(const Box3f& box, const glm::mat4& modelMatrix) const = 0;

    virtual glm::vec3 WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const = 0;
};

} // namespace hatcher
