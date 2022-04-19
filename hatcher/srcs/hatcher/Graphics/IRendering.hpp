#pragma once

#include <memory>

namespace hatcher
{
class MeshBuilder;
class World;

class IRendering
{
public:
    virtual ~IRendering() = default;

    virtual void UpdateWorldRendering(World* parWorld) = 0;
    virtual void RenderWorld() = 0;

    virtual const std::unique_ptr<MeshBuilder>& GetMeshBuilder() const = 0;
};

} // namespace hatcher
