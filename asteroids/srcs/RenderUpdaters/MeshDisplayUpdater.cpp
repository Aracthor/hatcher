#include "hatcher/Clock.hpp"
#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "RenderComponents/MeshComponent.hpp"

using namespace hatcher;

namespace
{

class MeshDisplayUpdater final : public RenderUpdater
{
public:
    MeshDisplayUpdater(const IRendering* rendering)
    {
        m_basicMaterial =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/position_2D.vert", "shaders/white.frag");
        m_instancedMaterial =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/instanced.vert", "shaders/white.frag");
        {
            m_meshes[MeshComponent::Player] = make_unique<Mesh>(m_instancedMaterial.get(), Primitive::Lines);

            float positions[] = {
                24.f,  0.f,

                -20.f, -16.f,

                -20.f, 16.f,

                -10.f, -12.f,

                -10.f, 12.f,
            };
            ushort indices[] = {0, 1, 0, 2, 3, 4};

            m_meshes[MeshComponent::Player]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::Player]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::Asteroid] = make_unique<Mesh>(m_instancedMaterial.get(), Primitive::LineStrip);

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

            m_meshes[MeshComponent::Asteroid]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::Asteroid]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::Shoot] = make_unique<Mesh>(m_instancedMaterial.get(), Primitive::LineStrip);

            float positions[] = {
                0.f,  2.f,

                2.f,  0.f,

                0.f,  -2.f,

                -2.f, 0.f,
            };
            ushort indices[] = {0, 1, 2, 3, 0};

            m_meshes[MeshComponent::Shoot]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::Shoot]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::UFO] = make_unique<Mesh>(m_instancedMaterial.get(), Primitive::Lines);

            float positions[] = {
                24.f,  0.f,

                -24.f, 0.f,

                12.f,  10.f,

                -12.f, 10.f,

                6.f,   20.f,

                -6.f,  20.f,

                12.f,  -10.f,

                -12.f, -10.f,
            };
            ushort indices[] = {0, 1, 0, 2, 1, 3, 2, 3, 2, 4, 3, 5, 4, 5, 0, 6, 1, 7, 6, 7};

            m_meshes[MeshComponent::UFO]->Set2DPositions(positions, std::size(positions));
            m_meshes[MeshComponent::UFO]->SetIndices(indices, std::size(indices));
        }
        {
            m_meshes[MeshComponent::WreckageAsteroid] =
                make_unique<Mesh>(m_instancedMaterial.get(), Primitive::LineStrip);

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
            m_meshes[MeshComponent::WreckageShip] = make_unique<Mesh>(m_instancedMaterial.get(), Primitive::Lines);

            float positions[] = {-24.f, 0.f, 24.f, 0.f};

            m_meshes[MeshComponent::WreckageShip]->Set2DPositions(positions, std::size(positions));
        }
        for (auto& mesh : m_meshes)
        {
            mesh->AddInstancedBuffer("inModelMatrix", 16);
        }

        {
            m_playerAcceleratingMesh = make_unique<Mesh>(m_basicMaterial.get(), Primitive::Lines);

            float positions[] = {
                -10.f, -6.f,

                -24.f, 0.f,

                -10.f, 8.f,
            };
            ushort indices[] = {0, 1, 1, 2};

            m_playerAcceleratingMesh->Set2DPositions(positions, std::size(positions));
            m_playerAcceleratingMesh->SetIndices(indices, std::size(indices));
        }
    }

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        frameRenderer.PrepareSceneDraw(m_basicMaterial.get());

        std::array<std::vector<glm::mat4>, MeshComponent::COUNT> reperesByMesh;

        auto meshComponents = renderComponentManager->ReadComponents<MeshComponent>();
        auto positionComponents = componentManager->ReadComponents<PositionComponent>();
        auto playerComponents = componentManager->ReadComponents<PlayerComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            const auto& meshComponent = meshComponents[i];
            const auto& positionComponent = positionComponents[i];
            const auto& playerComponent = playerComponents[i];
            if (meshComponent)
            {
                HATCHER_ASSERT(positionComponent);
                glm::mat4 modelMatrix = glm::mat4(1.f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(positionComponent->position, 0.f));
                modelMatrix = glm::rotate(modelMatrix, positionComponent->angle, glm::vec3(0.f, 0.f, 1.f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(meshComponent->scale));
                reperesByMesh[meshComponent->ID].push_back(modelMatrix);
                if (playerComponent && playerComponent->accelerating)
                {
                    m_acceleratingDisplayTime += frameRenderer.GetClock()->GetElapsedTime();
                    if (m_acceleratingDisplayTime > 30.f)
                    {
                        m_acceleratingDisplayTime = 0.f;
                        m_displayAcceleratingMesh = !m_displayAcceleratingMesh;
                    }
                    if (m_displayAcceleratingMesh)
                        m_playerAcceleratingMesh->Draw(modelMatrix);
                }
            }
        }

        frameRenderer.PrepareSceneDraw(m_instancedMaterial.get());
        for (int meshID = 0; meshID < (int)MeshComponent::COUNT; meshID++)
        {
            const std::vector<glm::mat4>& repereList = reperesByMesh[meshID];
            if (!repereList.empty())
            {
                const unique_ptr<Mesh>& mesh = m_meshes[meshID];
                mesh->FillInstancedBuffer("inModelMatrix", reinterpret_cast<const float*>(repereList.data()),
                                          repereList.size() * 16);
                mesh->DrawInstanced(repereList.size());
            }
        }
    }

    unique_ptr<Material> m_basicMaterial;
    unique_ptr<Material> m_instancedMaterial;
    unique_ptr<Mesh> m_meshes[MeshComponent::COUNT];

    unique_ptr<Mesh> m_playerAcceleratingMesh;
    float m_acceleratingDisplayTime = 0.f;
    bool m_displayAcceleratingMesh = false;
};

RenderUpdaterRegisterer<MeshDisplayUpdater> registerer;

} // namespace
