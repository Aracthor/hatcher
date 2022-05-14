#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/Graphics/Texture.hpp"
#include "hatcher/glm_pure.hpp"

#include "Position2DComponent.hpp"

namespace
{

class CubeDisplayUpdater final : public hatcher::RenderUpdater
{
public:
    CubeDisplayUpdater(const hatcher::IRendering* rendering)
    {
        // clang-format off
    float points[] =
    {
         0.5f,  0.5f, 0.f,
         0.5f, -0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        -0.5f,  0.5f, 0.f,

         0.5f,  0.5f, 1.f,
         0.5f, -0.5f, 1.f,
        -0.5f, -0.5f, 1.f,
        -0.5f,  0.5f, 1.f,

         0.5f,  0.5f, 0.f,
         0.5f, -0.5f, 0.f,
         0.5f, -0.5f, 1.f,
         0.5f,  0.5f, 1.f,

         0.5f, -0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        -0.5f, -0.5f, 1.f,
         0.5f, -0.5f, 1.f,

        -0.5f, -0.5f, 0.f,
        -0.5f,  0.5f, 0.f,
        -0.5f,  0.5f, 1.f,
        -0.5f, -0.5f, 1.f,

        -0.5f,  0.5f, 0.f,
         0.5f,  0.5f, 0.f,
         0.5f,  0.5f, 1.f,
        -0.5f,  0.5f, 1.f,
    };
    float textureCoords[] =
    {
        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,
    };
    hatcher::ushort indices[] =
    {
        0, 1, 2,
        0, 3, 2,

        4, 5, 6,
        4, 7, 6,

        8, 9, 10,
        8, 11,10,

        12,13,14,
        12,15,14,

        16,17,18,
        16,19,18,

        20,21,22,
        20,23,22,
    };
        // clang-format on
        rendering->GetMeshBuilder()->SetPrimitive(hatcher::Primitive::Triangles);
        std::shared_ptr<hatcher::Material> material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/hello_world_3D.vert",
                                                            "shaders/hello_texture.frag");

        m_texture = rendering->GetMaterialFactory()->TextureFromFile("textures/dirt.bmp");
        material->AddTexture("diffuseTexture", m_texture);
        rendering->GetMeshBuilder()->SetMaterial(material);

        m_mesh.reset(rendering->GetMeshBuilder()->Create());
        m_mesh->Set3DPositions(points, std::size(points));
        m_mesh->SetTextureCoords(textureCoords, std::size(textureCoords));
        m_mesh->SetIndices(indices, std::size(indices));
    }

    ~CubeDisplayUpdater() = default;

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                hatcher::IFrameRenderer& frameRenderer) override
    {
        glm::mat4 modelMatrix = glm::mat4(1.f);

        for (const std::optional<Position2DComponent> component :
             componentManager->GetComponents<Position2DComponent>())
        {
            if (component)
            {
                modelMatrix[3][0] = component->position.x;
                modelMatrix[3][1] = component->position.y;
                frameRenderer.AddMeshToRender(m_mesh.get(), modelMatrix);
            }
        }
    }

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
    std::shared_ptr<hatcher::Texture> m_texture;
};

const int dummy = hatcher::RegisterRenderUpdater<CubeDisplayUpdater>("CubeDisplay");

} // namespace
