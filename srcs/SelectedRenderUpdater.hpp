#pragma once

#include <memory>
#include <unordered_map>

#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/Maths/Box.hpp"

namespace hatcher
{
class IRendering;
class Mesh;
class MeshBuilder;
class World;
} // namespace hatcher

class SelectedRenderUpdater final : public hatcher::RenderUpdater
{
public:
    SelectedRenderUpdater(const hatcher::IRendering* rendering);
    ~SelectedRenderUpdater();

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
};
