#pragma once

#include <memory>

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

    virtual const std::unique_ptr<MeshBuilder>& GetMeshBuilder() const = 0;
};

} // namespace hatcher
