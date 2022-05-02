#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class IRendering;
class Mesh;
class Texture;
class World;
} // namespace hatcher

class CubeDisplayUpdater final : public hatcher::RenderUpdater
{
public:
    CubeDisplayUpdater(const hatcher::IRendering* rendering);
    ~CubeDisplayUpdater();

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
    std::shared_ptr<hatcher::Texture> m_texture;
};
