#include "RenderUpdaterOrder.hpp"

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

#include "Components/GrowableComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "RenderComponents/StaticMeshComponent.hpp"
#include "RenderComponents/SteveAnimationComponent.hpp"
#include "utils/TransformationHelper.hpp"

using namespace hatcher;

namespace
{

unique_ptr<Material> CreateTextureMaterial(MaterialFactory* materialFactory, const char* textureFileName)
{
    const Texture* texture = materialFactory->TextureFromFile(textureFileName);
    unique_ptr<Material> material = materialFactory->CreateMaterial("shaders/textured.vert", "shaders/textured.frag");
    material->AddTexture("uniTexture", texture);
    return material;
}

class StaticMeshRenderUpdater final : public RenderUpdater
{
public:
    StaticMeshRenderUpdater(const IRendering* rendering)
    {
        MeshLoader* meshLoader = rendering->GetMeshLoader().get();
        MaterialFactory* materialFactory = rendering->GetMaterialFactory().get();

        CreateTexturedMesh(meshLoader, materialFactory, StaticMeshComponent::Locker, "assets/meshes/locker.obj",
                           "assets/textures/locker.bmp");
        CreateTexturedMesh(meshLoader, materialFactory, StaticMeshComponent::Melon, "assets/meshes/melon.obj",
                           "assets/textures/melon.bmp");
        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Tree, "assets/meshes/tree.obj");
        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Wood, "assets/meshes/wood.obj");
    }

    ~StaticMeshRenderUpdater() = default;

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        const auto positionComponents = componentManager->ReadComponents<Position2DComponent>();
        const auto itemComponents = componentManager->ReadComponents<ItemComponent>();
        const auto growableComponents = componentManager->ReadComponents<GrowableComponent>();
        const auto steveAnimationComponents = renderComponentManager->ReadComponents<SteveAnimationComponent>();
        auto staticMeshComponents = renderComponentManager->WriteComponents<StaticMeshComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (staticMeshComponents[i])
            {
                HATCHER_ASSERT(staticMeshComponents[i]->type < StaticMeshComponent::COUNT);
                const unique_ptr<Mesh>& mesh = m_meshes[staticMeshComponents[i]->type];
                const unique_ptr<Material>& material = m_materials[staticMeshComponents[i]->type];

                std::optional<glm::mat4> modelMatrix;
                if (positionComponents[i])
                {
                    modelMatrix = TransformationHelper::ModelFromComponents(positionComponents[i]);
                    if (growableComponents[i])
                    {
                        modelMatrix = glm::scale(*modelMatrix, glm::vec3(growableComponents[i]->maturity));
                    }
                }
                else if (itemComponents[i] && itemComponents[i]->inventory)
                {
                    const Entity inventory = *itemComponents[i]->inventory;
                    const auto storagePosition = positionComponents[inventory];
                    if (storagePosition && steveAnimationComponents[inventory])
                    {
                        modelMatrix = TransformationHelper::ModelFromComponents(*storagePosition);
                        modelMatrix = glm::translate(*modelMatrix, glm::vec3(0.f, 0.f, 1.8f));
                    }
                }
                if (modelMatrix)
                {
                    frameRenderer.PrepareSceneDraw(material.get());
                    const int count = itemComponents[i] ? itemComponents[i]->count : 1;
                    HATCHER_ASSERT(count >= 1);
                    switch (count)
                    {
                    case 1:
                        mesh->Draw(*modelMatrix);
                        break;
                    case 2:
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(0.25f, 0.f, 0.f)));
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(-0.25f, 0.f, 0.f)));
                        break;
                    case 3:
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(0.25f, 0.25f, 0.f)));
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(-0.25f, 0.25f, 0.f)));
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(0.f, -0.25f, 0.f)));
                        break;
                    default:
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(0.f, 0.f, 0.5f)));
                    case 4:
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(0.25f, 0.25f, 0.f)));
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(0.25f, -0.25f, 0.f)));
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(-0.25f, 0.25f, 0.f)));
                        mesh->Draw(glm::translate(*modelMatrix, glm::vec3(-0.25f, -0.25f, 0.f)));
                        break;
                    }
                }
            }
        }
    }

    void OnCreateEntity(Entity entity, const ComponentManager* componentManager,
                        ComponentManager* renderComponentManager) override
    {
        const auto& staticMeshComponent = renderComponentManager->WriteComponents<StaticMeshComponent>()[entity];
        auto& selectableComponent = renderComponentManager->WriteComponents<SelectableComponent>()[entity];
        if (staticMeshComponent && selectableComponent)
        {
            const unique_ptr<Mesh>& mesh = m_meshes[staticMeshComponent->type];
            selectableComponent->box.Add(mesh->Box().Scaled(1.1f));
        }
    }

private:
    void CreateTexturedMesh(MeshLoader* meshLoader, MaterialFactory* materialFactory, StaticMeshComponent::Type type,
                            const char* meshFileName, const char* textureFileName)
    {
        m_materials[type] = CreateTextureMaterial(materialFactory, textureFileName);
        m_meshes[type] = meshLoader->LoadWavefront(m_materials[type].get(), meshFileName);
    }

    void CreateMesh(MeshLoader* meshLoader, MaterialFactory* materialFactory, StaticMeshComponent::Type type,
                    const char* meshFileName)
    {
        m_materials[type] = materialFactory->CreateMaterial("shaders/colored.vert", "shaders/colored.frag");
        m_meshes[type] = meshLoader->LoadWavefront(m_materials[type].get(), meshFileName);
    }

    unique_ptr<Material> m_materials[StaticMeshComponent::COUNT];
    unique_ptr<Mesh> m_meshes[StaticMeshComponent::COUNT];
};

RenderUpdaterRegisterer<StaticMeshRenderUpdater> registerer((int)ERenderUpdaterOrder::Scene);

} // namespace
