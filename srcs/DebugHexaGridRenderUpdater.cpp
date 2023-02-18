#include <memory>

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

using namespace hatcher;

namespace
{
float DegreeToRadian(float angle)
{
    return angle / 180.f * M_PI;
}

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

        std::vector<float> positions;
        const int hexagonCount = m_gridSize * m_gridSize * 4;
        positions.reserve(hexagonCount * 24);
        const glm::vec2 qVector = {sqrtf(3.f), 0.f};
        const glm::vec2 rVector = {sqrtf(3.f) / 2.f, 3.f / 2.f};
        for (int r = -m_gridSize; r < m_gridSize + 1; r++)
        {
            for (int q = -m_gridSize; q < m_gridSize + 1; q++)
            {
                const glm::vec2 hexagonCenter = float(q) * qVector + float(r) * rVector;
                for (int i = 0; i < 6; i++)
                {
                    const float angle = DegreeToRadian(30.f + float(i) * 60.f);
                    const float nextAngle = DegreeToRadian(30.f + float(i + 1) * 60.f);
                    positions.push_back((hexagonCenter.x + cosf(angle)) * m_hexaSize);
                    positions.push_back((hexagonCenter.y + sinf(angle)) * m_hexaSize);
                    positions.push_back((hexagonCenter.x + cosf(nextAngle)) * m_hexaSize);
                    positions.push_back((hexagonCenter.y + sinf(nextAngle)) * m_hexaSize);
                };
            }
        }

        m_gridMesh->Set2DPositions(positions.data(), std::size(positions));
    }

    ~DebugGridHexaRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        if (m_gridDisplayEnabled)
        {
            const glm::mat4 identityMatrix = glm::mat4(1.f);
            frameRenderer.AddMeshToRender(m_gridMesh.get(), identityMatrix);
        }
    }

private:
    bool m_gridDisplayEnabled = false;
    int m_gridSize = 20;
    float m_hexaSize = 1.f;
    std::unique_ptr<Mesh> m_gridMesh;
};

const int dummy = RegisterRenderUpdater<DebugGridHexaRenderUpdater>("DebugHexaGrid");

} // namespace