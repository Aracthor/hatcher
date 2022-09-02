#include <memory>

#include "GridDisplay.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{

class DebugGridEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_U)
        {
            GridDisplay* gridDisplay = renderComponentManager->WriteWorldComponent<GridDisplay>();
            gridDisplay->SetEnabled(!gridDisplay->Enabled());
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }
};

class DebugGridRenderUpdater final : public RenderUpdater
{
public:
    DebugGridRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<DebugGridEventListener>());

        MeshBuilder* meshBuilder = rendering->GetMeshBuilder().get();
        meshBuilder->SetPrimitive(Primitive::Lines);

        std::shared_ptr<Material> material = rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_2D.vert", "shaders/hello_color.frag");
        material->AddUniform("uniColor", glm::vec4(1.0, 1.0, 1.0, 0.2));
        meshBuilder->SetMaterial(material);
        m_gridMesh.reset(meshBuilder->Create());

        std::vector<float> positions;
        positions.reserve(m_gridSize * 8);
        for (int i = 0; i < m_gridSize; i++)
        {
            // x
            positions.push_back(i);
            positions.push_back(0);
            positions.push_back(i);
            positions.push_back(m_gridSize - 1);
            // y
            positions.push_back(0);
            positions.push_back(i);
            positions.push_back(m_gridSize - 1);
            positions.push_back(i);
        }

        m_gridMesh->Set2DPositions(positions.data(), std::size(positions));
    }

    ~DebugGridRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const GridDisplay* gridDisplay = renderComponentManager->ReadWorldComponent<GridDisplay>();
        if (gridDisplay->Enabled())
        {
            int gridPositionX = -m_gridSize / 2 + 1;
            int gridPositionY = -m_gridSize / 2 + 1;
            glm::mat4 modelMatrix = glm::translate(glm::vec3(gridPositionX, gridPositionY, 0.f));

            frameRenderer.AddMeshToRender(m_gridMesh.get(), modelMatrix);
        }
    }

private:
    int m_gridSize = 150;
    std::unique_ptr<Mesh> m_gridMesh;
};

const int dummy = RegisterRenderUpdater<DebugGridRenderUpdater>("DebugGrid");

} // namespace