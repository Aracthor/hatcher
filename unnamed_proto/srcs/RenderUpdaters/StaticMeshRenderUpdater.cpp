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
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "RenderComponents/StaticMeshComponent.hpp"
#include "TransformationHelper.hpp"

using namespace hatcher;

namespace
{

unique_ptr<Material> CreateTextureMaterial(MaterialFactory* materialFactory, const char* textureFileName)
{
    const Texture* texture = materialFactory->TextureFromFile(textureFileName);
    unique_ptr<Material> material =
        materialFactory->CreateMaterial("shaders/hello_world_3D.vert", "shaders/hello_texture.frag");
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

        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Locker, "assets/meshes/locker.obj",
                   "assets/textures/locker.bmp");
        CreateMesh(meshLoader, materialFactory, StaticMeshComponent::Melon, "assets/meshes/melon.obj",
                   "assets/textures/melon.bmp");
    }

    ~StaticMeshRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const auto positionComponents = componentManager->ReadComponents<Position2DComponent>();
        const auto growableComponents = componentManager->ReadComponents<GrowableComponent>();
        auto staticMeshComponents = renderComponentManager->WriteComponents<StaticMeshComponent>();
        auto selectableComponents = renderComponentManager->WriteComponents<SelectableComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (positionComponents[i] && staticMeshComponents[i])
            {
                HATCHER_ASSERT(staticMeshComponents[i]->type < StaticMeshComponent::COUNT);
                const unique_ptr<Mesh>& mesh = m_meshes[staticMeshComponents[i]->type];
                if (selectableComponents[i] && selectableComponents[i]->box.IsEmpty())
                {
                    selectableComponents[i]->box.Add(mesh->Box().Scaled(1.1f));
                }

                glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(positionComponents[i]);
                if (growableComponents[i])
                {
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(growableComponents[i]->maturity));
                }

                frameRenderer.AddMeshToRender(mesh.get(), modelMatrix);
            }
        }
    }

private:
    void CreateMesh(MeshLoader* meshLoader, MaterialFactory* materialFactory, StaticMeshComponent::Type type,
                    const char* meshFileName, const char* textureFileName)
    {
        m_materials[type] = CreateTextureMaterial(materialFactory, textureFileName);
        m_meshes[type] = meshLoader->LoadWavefront(m_materials[type].get(), meshFileName, Primitive::Triangles);
    }

    unique_ptr<Material> m_materials[StaticMeshComponent::COUNT];
    unique_ptr<Mesh> m_meshes[StaticMeshComponent::COUNT];
};

RenderUpdaterRegisterer<StaticMeshRenderUpdater> registerer;

} // namespace
