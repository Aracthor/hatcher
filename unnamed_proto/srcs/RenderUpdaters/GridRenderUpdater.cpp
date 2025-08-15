#include "RenderUpdaterOrder.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/Graphics/Texture.hpp"
#include "hatcher/assert.hpp"
#include "hatcher/unique_ptr.hpp"

#include "WorldComponents/SquareGrid.hpp"

using namespace hatcher;

namespace
{
bool gridDisplayEnabled = true;

class GridEventListener : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_J)
        {
            gridDisplayEnabled = !gridDisplayEnabled;
        }
    }
};

class GridRenderUpdater : public RenderUpdater
{
public:
    GridRenderUpdater(const IRendering* rendering)
    {
        MaterialFactory* materialFactory = rendering->GetMaterialFactory().get();

        m_gridMaterial = materialFactory->CreateMaterial("shaders/grounded.vert", "shaders/const_color.frag");
        m_gridMaterial->AddUniform("uniHeight", 0.01f);
        m_gridMaterial->AddUniform("uniColor", glm::vec4(0.2, 0.2, 0.2, 1.0));
        m_gridTileMesh = make_unique<Mesh>(m_gridMaterial.get(), Primitive::LineStrip);

        const Texture* texture = materialFactory->TextureFromFile("assets/textures/ground/grass.bmp");

        m_tileMaterial = materialFactory->CreateMaterial("shaders/gridtile.vert", "shaders/textured.frag");
        m_tileMaterial->AddTexture("uniTexture", texture);
        m_walkableTileMesh = make_unique<Mesh>(m_tileMaterial.get(), Primitive::TriangleStrip);

        FillGridMesh();
    }

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        const SquareGrid* grid = componentManager->ReadWorldComponent<SquareGrid>();

        for (int y = grid->GetTileCoordMin().y; y < grid->GetTileCoordMax().y; y++)
        {
            for (int x = grid->GetTileCoordMin().x; x < grid->GetTileCoordMax().x; x++)
            {
                const glm::vec2 tileCenter = grid->GetTileCenter({x, y});
                if (!grid->HasTileData(tileCenter))
                    continue;
                const SquareGrid::TileData& tileData = grid->GetTileData(tileCenter);
                if (tileData.walkable)
                {
                    const glm::mat4 tileMatrix = glm::translate(glm::vec3(tileCenter, 0.f));
                    frameRenderer.PrepareSceneDraw(m_tileMaterial.get());
                    m_walkableTileMesh->Draw(tileMatrix);
                    if (gridDisplayEnabled)
                    {
                        frameRenderer.PrepareSceneDraw(m_gridMaterial.get());
                        m_gridTileMesh->Draw(tileMatrix);
                    }
                }
            }
        }
    }

private:
    void FillGridMesh()
    {
        const float tilePositions[] = {
            -0.5f, -0.5f,

            -0.5f, 0.5f,

            0.5f,  -0.5f,

            0.5f,  0.5f,
        };
        const float linePositions[] = {
            -0.5f, -0.5f,

            -0.5f, 0.5f,

            0.5f,  0.5f,

            0.5f,  -0.5f,
        };
        m_walkableTileMesh->Set2DPositions(tilePositions, std::size(tilePositions));
        m_gridTileMesh->Set2DPositions(linePositions, std::size(linePositions));
    }

    unique_ptr<Material> m_gridMaterial;
    unique_ptr<Material> m_tileMaterial;
    unique_ptr<Mesh> m_gridTileMesh;
    unique_ptr<Mesh> m_walkableTileMesh;
};

EventListenerRegisterer<GridEventListener> eventRegisterer;
RenderUpdaterRegisterer<GridRenderUpdater> updaterRegisterer((int)ERenderUpdaterOrder::Scene);

} // namespace
