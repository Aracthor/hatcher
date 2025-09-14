#include "RenderUpdaterOrder.hpp"

#include "hatcher/ComponentAccessor.hpp"
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
#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/ItemDisplayComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "RenderComponents/StaticMeshComponent.hpp"
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

int ItemIndex(const InventoryComponent& inventory, Entity entity, ComponentReader<ItemComponent> itemComponents)
{
    const ItemComponent::EType itemType = itemComponents[entity]->type;
    int typeIndex = 0;
    int index = 0;
    while (inventory.storage[index] != entity)
    {
        Entity entity = inventory.storage[index];
        if (itemComponents[entity]->type == itemType)
            typeIndex++;
        index++;
    }
    return typeIndex;
}

class StaticMeshRenderUpdater final : public RenderUpdater
{
public:
    StaticMeshRenderUpdater(const IRendering* rendering)
    {
        MeshLoader* meshLoader = rendering->GetMeshLoader().get();
        MaterialFactory* materialFactory = rendering->GetMaterialFactory().get();

        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Axe, "assets/meshes/axe.obj");
        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Hut, "assets/meshes/hut.obj");
        CreateTexturedMesh(meshLoader, materialFactory, StaticMeshComponent::Melon, "assets/meshes/melon.obj",
                           "assets/textures/melon.bmp");
        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Rack, "assets/meshes/rack.obj");
        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Tree, "assets/meshes/tree.obj");
        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Wood, "assets/meshes/wood.obj");
    }

    ~StaticMeshRenderUpdater() = default;

    void Update(IApplication* application, const ComponentAccessor* componentAccessor,
                ComponentAccessor* renderComponentAccessor, IFrameRenderer& frameRenderer) override
    {
        const auto positionComponents = componentAccessor->ReadComponents<Position2DComponent>();
        const auto growableComponents = componentAccessor->ReadComponents<GrowableComponent>();
        const auto inventoryComponents = componentAccessor->ReadComponents<InventoryComponent>();
        const auto itemComponents = componentAccessor->ReadComponents<ItemComponent>();
        const auto itemDisplaysComponents = renderComponentAccessor->ReadComponents<ItemDisplayComponent>();
        auto staticMeshComponents = renderComponentAccessor->WriteComponents<StaticMeshComponent>();

        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            if (staticMeshComponents[i])
            {
                HATCHER_ASSERT(staticMeshComponents[i]->type < StaticMeshComponent::COUNT);
                const unique_ptr<Mesh>& mesh = m_meshes[staticMeshComponents[i]->type];
                const unique_ptr<Material>& material = m_materials[staticMeshComponents[i]->type];

                const auto positionComponent = positionComponents[i];
                const auto itemComponent = itemComponents[i];

                std::optional<glm::mat4> modelMatrix;
                if (positionComponent)
                {
                    modelMatrix = TransformationHelper::ModelFromComponents(*positionComponent);
                    if (growableComponents[i])
                    {
                        modelMatrix = glm::scale(*modelMatrix, glm::vec3(growableComponents[i]->maturity));
                    }
                }
                else if (itemComponent && itemComponent->inventory)
                {
                    const Entity inventory = *itemComponent->inventory;
                    const auto storagePosition = positionComponents[inventory];
                    if (storagePosition && itemDisplaysComponents[inventory])
                    {
                        const auto itemDisplayComponent = itemDisplaysComponents[inventory];
                        const auto inventoryComponent = *inventoryComponents[inventory];
                        const int itemIndex = ItemIndex(inventoryComponent, Entity(i), itemComponents);
                        const ItemDisplayComponent::LocationKey locationKey(itemComponent->type, itemIndex);
                        const auto it = itemDisplayComponent->locations.find(locationKey);
                        if (it != itemDisplayComponent->locations.end())
                        {
                            modelMatrix = TransformationHelper::ModelFromComponents(*storagePosition);
                            modelMatrix = *modelMatrix * it->second;
                        }
                    }
                }
                if (modelMatrix)
                {
                    frameRenderer.PrepareSceneDraw(material.get());
                    const int count = itemComponent ? itemComponent->count : 1;
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

    void OnCreateEntity(Entity entity, const ComponentAccessor* componentAccessor,
                        ComponentAccessor* renderComponentAccessor) override
    {
        const auto& staticMeshComponent = renderComponentAccessor->WriteComponents<StaticMeshComponent>()[entity];
        auto& selectableComponent = renderComponentAccessor->WriteComponents<SelectableComponent>()[entity];
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
