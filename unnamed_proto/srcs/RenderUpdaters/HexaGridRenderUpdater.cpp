#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/Graphics/Texture.hpp"
#include "hatcher/assert.hpp"
#include "hatcher/unique_ptr.hpp"

#include "WorldComponents/HexagonalGrid.hpp"

using namespace hatcher;

namespace
{
class HexaGridRenderUpdater final : public RenderUpdater
{
public:
    HexaGridRenderUpdater(const IRendering* rendering)
    {
        MaterialFactory* materialFactory = rendering->GetMaterialFactory().get();

        m_gridMaterial = materialFactory->CreateMaterial("shaders/grounded.vert", "shaders/const_color.frag");
        m_gridMaterial->AddUniform("uniHeight", 0.01f);
        m_gridMaterial->AddUniform("uniColor", glm::vec4(0.2, 0.2, 0.2, 1.0));
        m_gridTileMesh = make_unique<Mesh>(m_gridMaterial.get(), Primitive::Lines);

        const Texture* texture = materialFactory->TextureFromFile("assets/textures/ground/grass.bmp");

        m_tileMaterial = materialFactory->CreateMaterial("shaders/hexatile.vert", "shaders/textured.frag");
        m_tileMaterial->AddTexture("uniTexture", texture);
        m_walkableTileMesh = make_unique<Mesh>(m_tileMaterial.get(), Primitive::TriangleFan);

        FillGridMesh();
    }

    ~HexaGridRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const HexagonalGrid* grid = componentManager->ReadWorldComponent<HexagonalGrid>();

        for (int r = grid->GetTileCoordMin().r; r <= grid->GetTileCoordMax().r; r++)
        {
            for (int q = grid->GetTileCoordMin().q; q <= grid->GetTileCoordMax().q; q++)
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
                    if (m_gridDisplayEnabled)
                        frameRenderer.AddMeshToRender(m_gridTileMesh.get(), tileMatrix);
                }
            }
        }
    }

    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
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
    void FillGridMesh()
    {
        std::vector<float> tilePositions;
        tilePositions.reserve(12);
        for (int i = 0; i < 6; i++)
        {
            const glm::vec2 anglePosition = HexagonalGrid::GetHexaAngle(i);
            tilePositions.push_back(anglePosition.x);
            tilePositions.push_back(anglePosition.y);
        }
        m_walkableTileMesh->Set2DPositions(tilePositions.data(), std::size(tilePositions));
        m_gridTileMesh->Set2DPositions(tilePositions.data(), std::size(tilePositions));
    }

    bool m_gridDisplayEnabled = true;

    unique_ptr<Material> m_gridMaterial;
    unique_ptr<Material> m_tileMaterial;
    unique_ptr<Mesh> m_gridTileMesh;
    unique_ptr<Mesh> m_walkableTileMesh;
};

RenderUpdaterRegisterer<HexaGridRenderUpdater> registerer;

} // namespace
