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

#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "RenderComponents/StaticMeshComponent.hpp"
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

        const Texture* lockerTexture = materialFactory->TextureFromFile("assets/textures/locker.bmp");

        m_fillMaterial = materialFactory->CreateMaterial("shaders/hello_world_3D.vert", "shaders/hello_texture.frag");
        m_fillMaterial->AddTexture("uniTexture", lockerTexture);

        const char* lockerFileName = "assets/meshes/locker.obj";
        m_meshes[StaticMeshComponent::Locker] =
            meshLoader->LoadWavefront(m_fillMaterial.get(), lockerFileName, Primitive::Triangles);
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
                const unique_ptr<Mesh>& mesh = m_meshes[staticMeshComponents[i]->type];
                if (selectableComponents[i] && selectableComponents[i]->box.IsEmpty())
                {
                    selectableComponents[i]->box.Add(mesh->Box().Scaled(1.1f));
                }

                glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(positionComponents[i]);
                frameRenderer.AddMeshToRender(mesh.get(), modelMatrix);
            }
        }
    }

private:
    unique_ptr<Material> m_fillMaterial;
    unique_ptr<Mesh> m_meshes[StaticMeshComponent::COUNT] = {};
};

RenderUpdaterRegisterer<StaticMeshRenderUpdater> registerer;

} // namespace
