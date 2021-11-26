#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class Mesh;
class MeshBuilder;
class World;
} // namespace hatcher

class SquareDisplayUpdater final : public hatcher::RenderUpdater
{
public:
    SquareDisplayUpdater(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~SquareDisplayUpdater();

    void Update(hatcher::ComponentManager* componentManager,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
};
