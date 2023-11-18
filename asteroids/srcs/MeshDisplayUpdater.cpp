#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/Clock.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

#include "AsteroidComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"

using namespace hatcher;

namespace
{

class MeshDisplayUpdater final : public RenderUpdater
{
public:
    MeshDisplayUpdater(const IRendering* rendering)
    {
        m_material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/hello_world.vert", "shaders/hello_world.frag");
        {
            m_playerMesh = make_unique<Mesh>(m_material.get(), Primitive::Lines);

            float positions[] = {
                24.f,  0.f,

                -20.f, -16.f,

                -20.f, 16.f,

                -10.f, -12.f,

                -10.f, 12.f,
            };
            ushort indices[] = {0, 1, 0, 2, 3, 4};

            m_playerMesh->Set2DPositions(positions, std::size(positions));
            m_playerMesh->SetIndices(indices, std::size(indices));
        }
        {
            m_asteroidMesh = make_unique<Mesh>(m_material.get(), Primitive::LineStrip);

            float positions[] = {
                10.f,  50.f,

                10.f,  20.f,

                45.f,  20.f,

                45.f,  -20.f,

                15.f,  -25.f,

                0.f,   -50.f,

                -20.f, -45.f,

                -45.f, -20.f,

                -45.f, 20.f,

                -15.f, 50.f,
            };
            ushort indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

            m_asteroidMesh->Set2DPositions(positions, std::size(positions));
            m_asteroidMesh->SetIndices(indices, std::size(indices));
        }
        {
            m_playerAcceleratingMesh = make_unique<Mesh>(m_material.get(), Primitive::Lines);

            float positions[] = {
                -10.f, -6.f,

                -24.f, 0.f,

                -10.f, 6.f,
            };
            ushort indices[] = {0, 1, 1, 2};

            m_playerAcceleratingMesh->Set2DPositions(positions, std::size(positions));
            m_playerAcceleratingMesh->SetIndices(indices, std::size(indices));
        }
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        auto asteroidComponents = componentManager->ReadComponents<AsteroidComponent>();
        auto positionComponents = componentManager->ReadComponents<PositionComponent>();
        auto playerComponents = componentManager->ReadComponents<PlayerComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            const auto& positionComponent = positionComponents[i];
            const auto& playerComponent = playerComponents[i];
            if (positionComponent)
            {
                glm::mat4 modelMatrix = glm::mat4(1.f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(positionComponent->position, 0.f));
                modelMatrix = glm::rotate(modelMatrix, positionComponent->angle, glm::vec3(0.f, 0.f, 1.f));
                if (playerComponent)
                {
                    frameRenderer.AddMeshToRender(m_playerMesh.get(), modelMatrix);
                    if (playerComponent->accelerating)
                    {
                        m_acceleratingDisplayTime += frameRenderer.GetClock()->GetElapsedTime();
                        if (m_acceleratingDisplayTime > 30.f)
                        {
                            m_acceleratingDisplayTime = 0.f;
                            m_displayAcceleratingMesh = !m_displayAcceleratingMesh;
                        }
                        if (m_displayAcceleratingMesh)
                            frameRenderer.AddMeshToRender(m_playerAcceleratingMesh.get(), modelMatrix);
                    }
                }
                else if (asteroidComponents[i])
                {
                    frameRenderer.AddMeshToRender(m_asteroidMesh.get(), modelMatrix);
                }
            }
        }
    }

    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_playerMesh;
    unique_ptr<Mesh> m_asteroidMesh;

    unique_ptr<Mesh> m_playerAcceleratingMesh;
    float m_acceleratingDisplayTime = 0.f;
    bool m_displayAcceleratingMesh = false;
};

RenderUpdaterRegisterer<MeshDisplayUpdater> registerer;

} // namespace
