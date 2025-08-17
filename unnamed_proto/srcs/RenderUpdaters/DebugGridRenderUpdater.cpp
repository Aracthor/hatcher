#include "RenderUpdaterOrder.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"
#include "hatcher/unique_ptr.hpp"

using namespace hatcher;

namespace
{
bool gridDisplayEnabled = false;

class DebugGridEventListener final : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_U)
        {
            gridDisplayEnabled = !gridDisplayEnabled;
        }
    }
};

class DebugGridRenderUpdater final : public RenderUpdater
{
public:
    DebugGridRenderUpdater(const IRendering* rendering)
    {
        m_material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/grounded.vert", "shaders/const_color.frag");
        m_material->AddUniform("uniColor", glm::vec4(1.0, 1.0, 1.0, 0.2));
        m_gridMesh = make_unique<Mesh>(m_material.get(), Primitive::Lines);

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

    void Update(IApplication* application, const ComponentAccessor* componentAccessor,
                ComponentAccessor* renderComponentAccessor, IFrameRenderer& frameRenderer) override
    {
        if (gridDisplayEnabled)
        {
            int gridPositionX = -m_gridSize / 2 + 1;
            int gridPositionY = -m_gridSize / 2 + 1;
            glm::mat4 modelMatrix = glm::translate(glm::vec3(gridPositionX, gridPositionY, 0.f));

            frameRenderer.PrepareSceneDraw(m_material.get());
            m_gridMesh->Draw(modelMatrix);
        }
    }

private:
    int m_gridSize = 150;
    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_gridMesh;
};

EventListenerRegisterer<DebugGridEventListener> eventRegisterer;
RenderUpdaterRegisterer<DebugGridRenderUpdater> updaterRegisterer((int)ERenderUpdaterOrder::Scene);

} // namespace