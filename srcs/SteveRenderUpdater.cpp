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
#include "SelectableComponent.hpp"
#include "TransformationHelper.hpp"

using namespace hatcher;

namespace
{

class SteveRenderUpdater final : public RenderUpdater
{
public:
    SteveRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
        : m_bodyParts({&m_torso, &m_head, &m_leftArm, &m_rightArm, &m_leftLeg, &m_rightLeg})
    {
        std::shared_ptr<Material> material = rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_3D.vert", "shaders/hello_texture.frag");

        m_texture = rendering->GetMaterialFactory()->TextureFromFile("assets/textures/skins/steve.bmp");
        material->AddTexture("diffuseTexture", m_texture);

        m_torso.mesh.reset(rendering->GetMeshLoader()->LoadWavefront(material, "assets/meshes/steve/torso.obj"));
        m_head.mesh.reset(rendering->GetMeshLoader()->LoadWavefront(material, "assets/meshes/steve/head.obj"));
        m_leftArm.mesh.reset(rendering->GetMeshLoader()->LoadWavefront(material, "assets/meshes/steve/left_arm.obj"));
        m_rightArm.mesh.reset(rendering->GetMeshLoader()->LoadWavefront(material, "assets/meshes/steve/right_arm.obj"));
        m_leftLeg.mesh.reset(rendering->GetMeshLoader()->LoadWavefront(material, "assets/meshes/steve/left_leg.obj"));
        m_rightLeg.mesh.reset(rendering->GetMeshLoader()->LoadWavefront(material, "assets/meshes/steve/right_leg.obj"));

        m_torso.matrix = glm::mat4(1.f);
        m_head.matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 1.2f));
        m_leftArm.matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.3f, 1.1f));
        m_rightArm.matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -0.3f, 1.1f));
        m_leftLeg.matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.1f, 0.6f));
        m_rightLeg.matrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -0.1f, 0.6f));
    }

    ~SteveRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const auto positionComponents = componentManager->ReadComponents<Position2DComponent>();
        const auto movementComponents = componentManager->ReadComponents<Movement2DComponent>();
        auto selectableComponents = renderComponentManager->WriteComponents<SelectableComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (positionComponents[i] && movementComponents[i])
            {
                if (selectableComponents[i] && selectableComponents[i]->box.IsPoint())
                {
                    for (const BodyPart* bodyPart : m_bodyParts)
                    {
                        const Box3f box = bodyPart->mesh->Box().Translated(glm::vec3(bodyPart->matrix[3]));
                        selectableComponents[i]->box.Add(box.Scaled(1.1f));
                    }
                }

                const glm::mat4 modelMatrix =
                    TransformationHelper::ModelFromComponents(positionComponents[i], movementComponents[i]);
                for (const BodyPart* bodyPart : m_bodyParts)
                {
                    frameRenderer.AddMeshToRender(bodyPart->mesh.get(), modelMatrix * bodyPart->matrix);
                }
            }
        }
    }

private:
    struct BodyPart
    {
        std::unique_ptr<Mesh> mesh;
        glm::mat4 matrix;
    };

    BodyPart m_torso;
    BodyPart m_head;
    BodyPart m_leftArm;
    BodyPart m_rightArm;
    BodyPart m_leftLeg;
    BodyPart m_rightLeg;
    std::array<BodyPart*, 6> m_bodyParts;
    std::shared_ptr<Texture> m_texture;
};

const int dummy = RegisterRenderUpdater<SteveRenderUpdater>();

} // namespace
