#include <memory>

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "HexagonalGrid.hpp"

using namespace hatcher;

namespace
{
class DebugHexaGridEventListener final : public IEventListener
{
public:
    DebugHexaGridEventListener(bool& gridDisplayEnabled)
        : m_gridDisplayEnabled(gridDisplayEnabled)
    {
    }

    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_J)
        {
            m_gridDisplayEnabled = !m_gridDisplayEnabled;
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }

private:
    bool& m_gridDisplayEnabled;
};

class DebugGridHexaRenderUpdater final : public RenderUpdater
{
public:
    DebugGridHexaRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(
            std::make_shared<DebugHexaGridEventListener>(m_gridDisplayEnabled));

        std::shared_ptr<Material> material = rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_2D.vert", "shaders/hello_color.frag");
        material->AddUniform("uniColor", glm::vec4(1.0, 1.0, 1.0, 0.2));
        m_gridMesh = std::make_unique<Mesh>(material, Primitive::Lines);
    }

    ~DebugGridHexaRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        if (!m_meshFilled)
        {
            const HexagonalGrid* grid = componentManager->ReadWorldComponent<HexagonalGrid>();
            FillGridMesh(grid);
        }

        if (m_gridDisplayEnabled)
        {
            const glm::mat4 identityMatrix = glm::mat4(1.f);
            frameRenderer.AddMeshToRender(m_gridMesh.get(), identityMatrix);
        }
    }

private:
    void FillGridMesh(const HexagonalGrid* grid)
    {
        std::vector<float> positions;
        const int gridSize = grid->GridSize();
        const int hexagonCount = gridSize * gridSize * 4;
        positions.reserve(hexagonCount * 24);
        for (int r = -gridSize; r < gridSize + 1; r++)
        {
            for (int q = -gridSize; q < gridSize + 1; q++)
            {
                HexagonalGrid::TileCoord coord(q, r);
                for (int i = 0; i < 6; i++)
                {
                    const glm::vec2 anglePosition = grid->GetHexaAngle(coord, i);
                    const glm::vec2 nextAnglePosition = grid->GetHexaAngle(coord, i + 1);
                    positions.push_back(anglePosition.x);
                    positions.push_back(anglePosition.y);
                    positions.push_back(nextAnglePosition.x);
                    positions.push_back(nextAnglePosition.y);
                };
            }
        }

        m_gridMesh->Set2DPositions(positions.data(), std::size(positions));
    }

    bool m_gridDisplayEnabled = false;
    bool m_meshFilled = false;
    std::unique_ptr<Mesh> m_gridMesh;
};

const int dummy = RegisterRenderUpdater<DebugGridHexaRenderUpdater>("DebugHexaGrid");

} // namespace
