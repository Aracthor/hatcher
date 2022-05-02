#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class Mesh;
class MeshBuilder;
class Texture;
class World;
} // namespace hatcher

class CubeDisplayUpdater final : public hatcher::RenderUpdater
{
public:
    CubeDisplayUpdater(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~CubeDisplayUpdater();

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                const hatcher::IRendering& rendering,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
    std::shared_ptr<hatcher::Texture> m_texture;
};
