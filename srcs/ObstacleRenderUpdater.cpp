#include "ObstacleRenderUpdater.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/glm_pure.hpp"

#include "Obstacle2DComponent.hpp"

ObstacleRenderUpdater::ObstacleRenderUpdater(
    const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
    : m_meshBuilder(meshBuilder)
{
}

ObstacleRenderUpdater::~ObstacleRenderUpdater() = default;

void ObstacleRenderUpdater::Update(const hatcher::ComponentManager* componentManager,
                                   hatcher::ComponentManager* renderComponentManager,
                                   const hatcher::Clock& clock,
                                   hatcher::IFrameRenderer& frameRenderer)
{
    (void)clock;
    (void)renderComponentManager;
    (void)frameRenderer;

    for (const std::optional<Obstacle2DComponent> component :
         componentManager->GetComponents<Obstacle2DComponent>())
    {
    }
}
