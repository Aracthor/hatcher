#pragma once

#include <memory>

namespace hatcher
{
class MeshBuilder;
class World;

class IRendering
{
public:
    IRendering() = default;

    virtual void RenderWorld(World* parWorld) = 0;

    virtual const std::unique_ptr<MeshBuilder>& GetMeshBuilder() = 0;
};

} // namespace hatcher
