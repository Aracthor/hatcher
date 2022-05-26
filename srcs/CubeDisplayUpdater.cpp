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

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"

namespace
{

template <std::size_t N>
std::array<hatcher::ushort, N / 2 * 3> MakeIndicesFromRectVertices()
{
    int index = 0;
    std::array<hatcher::ushort, N / 2 * 3> result;
    for (std::size_t i = 0; i < N; i += 4)
    {
        result[index++] = i + 0;
        result[index++] = i + 1;
        result[index++] = i + 2;
        result[index++] = i + 0;
        result[index++] = i + 3;
        result[index++] = i + 2;
    }
    return result;
}

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
            24.f,  8.f,
            16.f,  8.f,
            16.f,  0.f,
            24.f,  0.f,

            16.f,  8.f,
             8.f,  8.f,
             8.f,  0.f,
            16.f,  0.f,

            16.f, 16.f,
             8.f, 16.f,
             8.f,  8.f,
            16.f,  8.f,

             8.f, 16.f,
             0.f, 16.f,
             0.f,  8.f,
             8.f,  8.f,

            32.f, 16.f,
            24.f, 16.f,
            24.f,  8.f,
            32.f,  8.f,

            24.f, 16.f,
            16.f, 16.f,
            16.f,  8.f,
            24.f,  8.f,
        };
        // clang-format on

        for (auto& textureCoord : textureCoords)
            textureCoord /= 64.f;
        auto indices = MakeIndicesFromRectVertices<std::size(points)>();

        rendering->GetMeshBuilder()->SetPrimitive(hatcher::Primitive::Triangles);
        std::shared_ptr<hatcher::Material> material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/hello_world_3D.vert",
                                                            "shaders/hello_texture.frag");

        m_texture = rendering->GetMaterialFactory()->TextureFromFile("textures/skins/steve.bmp");
        material->AddTexture("diffuseTexture", m_texture);
        rendering->GetMeshBuilder()->SetMaterial(material);

        m_mesh.reset(rendering->GetMeshBuilder()->Create());
        m_mesh->Set3DPositions(points, std::size(points));
        m_mesh->SetTextureCoords(textureCoords, std::size(textureCoords));
        m_mesh->SetIndices(indices.data(), std::size(indices));
    }

    ~CubeDisplayUpdater() = default;

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                hatcher::IFrameRenderer& frameRenderer) override
    {
        const auto positionComponents = componentManager->GetComponents<Position2DComponent>();
        const auto movementComponents = componentManager->GetComponents<Movement2DComponent>();

        for (size_t i = 0; i < positionComponents.size(); i++)
        {
            if (positionComponents[i] && movementComponents[i])
            {
                const Position2DComponent& position2D = *positionComponents[i];
                const Movement2DComponent& movement2D = *movementComponents[i];

                const float angle = glm::orientedAngle(glm::vec2(1.f, 0.f), movement2D.orientation);
                glm::mat4 modelMatrix = glm::mat4(1.f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(position2D.position, 0.f));
                modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.f, 0.f, 1.f));
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
