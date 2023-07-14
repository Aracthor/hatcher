#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshLoader.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/Graphics/Texture.hpp"
#include "hatcher/glm_pure.hpp"

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "TransformationHelper.hpp"

using namespace hatcher;

namespace
{

class MeshDisplayUpdater final : public RenderUpdater
{
public:
    MeshDisplayUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        std::shared_ptr<Material> material = rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_3D.vert", "shaders/hello_texture.frag");

        m_texture =
            rendering->GetMaterialFactory()->TextureFromFile("assets/textures/skins/steve.bmp");
        material->AddTexture("diffuseTexture", m_texture);

        m_mesh.reset(
            rendering->GetMeshLoader()->LoadWavefront(material, "assets/meshes/steve.obj"));
    }

    ~MeshDisplayUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const auto positionComponents = componentManager->ReadComponents<Position2DComponent>();
        const auto movementComponents = componentManager->ReadComponents<Movement2DComponent>();
        auto selectableComponents =
            renderComponentManager->WriteComponents<Selectable2DComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (positionComponents[i] && movementComponents[i])
            {
                if (selectableComponents[i] && selectableComponents[i]->box.IsPoint())
                    selectableComponents[i]->box = m_mesh->Box().Scaled(1.1f);

                const glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(
                    positionComponents[i], movementComponents[i]);
                frameRenderer.AddMeshToRender(m_mesh.get(), modelMatrix);
            }
        }
    }

private:
    std::unique_ptr<Mesh> m_mesh;
    std::shared_ptr<Texture> m_texture;
};

const int dummy = RegisterRenderUpdater<MeshDisplayUpdater>();

} // namespace
