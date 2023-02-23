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
class HexaGridEventListener final : public IEventListener
{
public:
    HexaGridEventListener(bool& gridDisplayEnabled)
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

class HexaGridRenderUpdater final : public RenderUpdater
{
public:
    HexaGridRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(
            std::make_shared<HexaGridEventListener>(m_gridDisplayEnabled));

        std::shared_ptr<Material> gridMaterial = rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_2D.vert", "shaders/hello_color.frag");
        gridMaterial->AddUniform("uniHeight", 0.01f);
        gridMaterial->AddUniform("uniColor", glm::vec4(0.2, 0.2, 0.2, 1.0));
        m_gridTileMesh = std::make_unique<Mesh>(gridMaterial, Primitive::Lines);

        std::shared_ptr<Material> tileMaterial = rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_2D.vert", "shaders/hello_color.frag");
        tileMaterial->AddUniform("uniHeight", 0.f);
        tileMaterial->AddUniform("uniColor", glm::vec4(0.3, 0.3, 0.3, 1.0));
        m_walkableTileMesh = std::make_unique<Mesh>(tileMaterial, Primitive::TriangleFan);
    }

    ~HexaGridRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const HexagonalGrid* grid = componentManager->ReadWorldComponent<HexagonalGrid>();
        if (!m_meshFilled)
        {
            FillGridMesh(grid);
        }

        if (m_gridDisplayEnabled)
        {
            for (int r = -m_gridDisplaySize; r < m_gridDisplaySize + 1; r++)
            {
                for (int q = -m_gridDisplaySize; q < m_gridDisplaySize + 1; q++)
                {
                    HexagonalGrid::TileCoord coord(q, r);
                    if (!grid->HasTileData(coord))
                        continue;
                    const HexagonalGrid::TileData& tileData = grid->GetTileData(coord);
                    if (tileData.walkable)
                    {
                        const glm::vec2 tileCenter = grid->TileCoordToPosition(coord);
                        const glm::mat4 tileMatrix = glm::translate(glm::vec3(tileCenter, 0.f));
                        frameRenderer.AddMeshToRender(m_walkableTileMesh.get(), tileMatrix);
                        frameRenderer.AddMeshToRender(m_gridTileMesh.get(), tileMatrix);
                    }
                }
            }
        }
    }

private:
    void FillGridMesh(const HexagonalGrid* grid)
    {
        HATCHER_ASSERT(m_meshFilled == false);
        m_meshFilled = true;

        std::vector<float> tilePositions;
        tilePositions.reserve(12);
        const HexagonalGrid::TileCoord centerCoord(0, 0);
        for (int i = 0; i < 6; i++)
        {
            const glm::vec2 anglePosition = grid->GetHexaAngle(centerCoord, i);
            tilePositions.push_back(anglePosition.x);
            tilePositions.push_back(anglePosition.y);
        }
        m_walkableTileMesh->Set2DPositions(tilePositions.data(), std::size(tilePositions));
        m_gridTileMesh->Set2DPositions(tilePositions.data(), std::size(tilePositions));
    }

    bool m_gridDisplayEnabled = true;
    bool m_meshFilled = false;
    int m_gridDisplaySize = 10;

    std::unique_ptr<Mesh> m_gridTileMesh;
    std::unique_ptr<Mesh> m_walkableTileMesh;
};

const int dummy = RegisterRenderUpdater<HexaGridRenderUpdater>("HexaGrid");

} // namespace
