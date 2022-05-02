#pragma once

#include <memory>

#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class Clock;
class MaterialFactory;
class MeshBuilder;
class World;

class IRendering
{
public:
    virtual ~IRendering() = default;

    virtual const Clock* GetClock() const = 0;

    virtual const std::unique_ptr<MaterialFactory>& GetMaterialFactory() const = 0;
    virtual const std::unique_ptr<MeshBuilder>& GetMeshBuilder() const = 0;
};

} // namespace hatcher
