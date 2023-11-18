#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshLoader.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/Graphics/Texture.hpp"
#include "hatcher/Maths/glm_pure.hpp"

#include <utility> // std::pair

#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"
#include "StaticMeshComponent.hpp"
#include "TransformationHelper.hpp"

using namespace hatcher;

namespace
{

class StaticMeshRenderUpdater final : public RenderUpdater
{
public:
    StaticMeshRenderUpdater(const IRendering* rendering)
    {
        MeshLoader* meshLoader = rendering->GetMeshLoader().get();
        MaterialFactory* materialFactory = rendering->GetMaterialFactory().get();

        m_texture = materialFactory->TextureFromFile("assets/textures/locker.bmp");

        m_fillMaterial = materialFactory->CreateMaterial("shaders/hello_world_3D.vert", "shaders/hello_texture.frag");
        m_fillMaterial->AddTexture("diffuseTexture", m_texture);
        m_lineMaterial = materialFactory->CreateMaterial("shaders/hello_world_3D.vert", "shaders/hello_color.frag");
        m_lineMaterial->AddUniform("uniColor", glm::vec4(0.0, 0.0, 0.0, 1.0));

        const char* lockerFileName = "assets/meshes/locker.obj";
        StaticMeshes& lockerMeshes = m_meshes[StaticMeshComponent::Locker];
        lockerMeshes.first = meshLoader->LoadWavefront(m_fillMaterial.get(), lockerFileName, Primitive::Triangles);
        lockerMeshes.second = meshLoader->LoadWavefront(m_lineMaterial.get(), lockerFileName, Primitive::Lines);
    }

    ~StaticMeshRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const auto positionComponents = componentManager->ReadComponents<Position2DComponent>();
        auto staticMeshComponents = renderComponentManager->WriteComponents<StaticMeshComponent>();
        auto selectableComponents = renderComponentManager->WriteComponents<SelectableComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (positionComponents[i] && staticMeshComponents[i])
            {
                HATCHER_ASSERT(staticMeshComponents[i]->type < StaticMeshComponent::COUNT);
                const StaticMeshes& meshes = m_meshes[staticMeshComponents[i]->type];
                if (selectableComponents[i] && selectableComponents[i]->box.IsEmpty())
                {
                    selectableComponents[i]->box.Add(meshes.first->Box().Scaled(1.1f));
                }

                glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(positionComponents[i]);
                frameRenderer.AddMeshToRender(meshes.first.get(), modelMatrix);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.01f, 1.01f, 1.01f));
                frameRenderer.AddMeshToRender(meshes.second.get(), modelMatrix);
            }
        }
    }

private:
    using StaticMeshes = std::pair<unique_ptr<Mesh>, unique_ptr<Mesh>>;

    const Texture* m_texture = nullptr;
    unique_ptr<Material> m_fillMaterial;
    unique_ptr<Material> m_lineMaterial;
    StaticMeshes m_meshes[StaticMeshComponent::COUNT] = {};
};

RenderUpdaterRegisterer<StaticMeshRenderUpdater> registerer;

} // namespace
