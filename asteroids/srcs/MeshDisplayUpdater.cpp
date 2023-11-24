#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/Clock.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

#include "CollidableComponent.hpp"
#include "MeshComponent.hpp"
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
            m_meshes[MeshComponent::Player] = make_unique<Mesh>(m_material.get(), Primitive::Lines);

            float positions[] = {
                1.f,    0.f,

                -0.83f, -0.67f,

                -0.83f, 0.67f,

                -0.42f, -0.5f,

                -0.42f, 0.5f,
            };
            ushort indices[] = {0, 1, 0, 2, 3, 4};

            m_meshes[MeshComponent::Player]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::Player]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::Asteroid] = make_unique<Mesh>(m_material.get(), Primitive::LineStrip);

            float positions[] = {
                0.2f,  1.f,

                0.2f,  0.4f,

                0.9f,  0.4f,

                0.9f,  -0.4f,

                0.3f,  -0.5f,

                0.f,   -1.f,

                -0.4f, -0.9f,

                -0.9f, -0.4f,

                -0.9f, 0.4f,

                -0.3f, 1.f,
            };
            ushort indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

            m_meshes[MeshComponent::Asteroid]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::Asteroid]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::Shoot] = make_unique<Mesh>(m_material.get(), Primitive::LineStrip);

            float positions[] = {
                0.f,  1.f,

                1.f,  0.f,

                0.f,  -1.f,

                -1.f, 0.f,
            };
            ushort indices[] = {0, 1, 2, 3, 0};

            m_meshes[MeshComponent::Shoot]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::Shoot]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::WreckageAsteroid] = make_unique<Mesh>(m_material.get(), Primitive::LineStrip);

            float positions[] = {
                0.f,  2.f,

                2.f,  0.f,

                0.f,  -2.f,

                -2.f, 0.f,
            };
            ushort indices[] = {0, 1, 2, 3, 0};

            m_meshes[MeshComponent::WreckageAsteroid]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::WreckageAsteroid]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::WreckageShip] = make_unique<Mesh>(m_material.get(), Primitive::Lines);

            float positions[] = {-24.f, 0.f, 24.f, 0.f};

            m_meshes[MeshComponent::WreckageShip]->Set2DPositions(positions, std::size(positions));
        }
        {
            m_playerAcceleratingMesh = make_unique<Mesh>(m_material.get(), Primitive::Lines);

            float positions[] = {
                -0.42f, -0.25f,

                -1.f,   0.f,

                -0.42f, 0.25f,
            };
            ushort indices[] = {0, 1, 1, 2};

            m_playerAcceleratingMesh->Set2DPositions(positions, std::size(positions));
            m_playerAcceleratingMesh->SetIndices(indices, std::size(indices));
        }
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        auto meshComponents = renderComponentManager->ReadComponents<MeshComponent>();
        auto collidableComponents = componentManager->ReadComponents<CollidableComponent>();
        auto positionComponents = componentManager->ReadComponents<PositionComponent>();
        auto playerComponents = componentManager->ReadComponents<PlayerComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            const auto& meshComponent = meshComponents[i];
            const auto& collidableComponent = collidableComponents[i];
            const auto& positionComponent = positionComponents[i];
            const auto& playerComponent = playerComponents[i];
            if (meshComponent)
            {
                HATCHER_ASSERT(positionComponent);
                glm::mat4 modelMatrix = glm::mat4(1.f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(positionComponent->position, 0.f));
                modelMatrix = glm::rotate(modelMatrix, positionComponent->angle, glm::vec3(0.f, 0.f, 1.f));
                if (collidableComponent)
                {
                    const float scale = collidableComponent->size;
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
                }
                frameRenderer.AddMeshToRender(m_meshes[meshComponent->ID].get(), modelMatrix);
                if (playerComponent && playerComponent->accelerating)
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
        }
    }

    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_meshes[MeshComponent::COUNT];

    unique_ptr<Mesh> m_playerAcceleratingMesh;
    float m_acceleratingDisplayTime = 0.f;
    bool m_displayAcceleratingMesh = false;
};

RenderUpdaterRegisterer<MeshDisplayUpdater> registerer;

} // namespace
