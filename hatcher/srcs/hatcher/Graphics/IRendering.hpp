#pragma once

#include <memory>

#include "hatcher/Maths/Box.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class Clock;
class MeshBuilder;
class World;

class IRendering
{
public:
    virtual ~IRendering() = default;

    virtual glm::ivec2 Resolution() const = 0;
    virtual const Clock* GetClock() const = 0;

    virtual glm::vec2 WorldCoordsToWindowCoords(const glm::vec3& worldCoords,
                                                const glm::mat4& modelMatrix) const = 0;
    virtual hatcher::Box2f ProjectBox3DToWindowCoords(const hatcher::Box3f& box,
                                                      const glm::mat4& modelMatrix) const = 0;

    virtual glm::vec3 WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const = 0;

    virtual const std::unique_ptr<MeshBuilder>& GetMeshBuilder() const = 0;
};

} // namespace hatcher
