#include <memory>

#include "SelectionRectangle.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/FrameRenderer.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{

class SelectionRectangleRenderUpdater final : public RenderUpdater
{
public:
    SelectionRectangleRenderUpdater(const IRendering* rendering)
    {
        MeshBuilder* meshBuilder = rendering->GetMeshBuilder().get();
        meshBuilder->SetPrimitive(Primitive::Lines);

        meshBuilder->SetMaterial(rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_2D.vert", "shaders/hello_world.frag"));
        m_selectionRectangleMesh.reset(meshBuilder->Create());

        float positions[] = {
            -1.f, -1.f,

            1.f,  -1.f,

            1.f,  1.f,

            -1.f, 1.f,
        };
        ushort indices[] = {0, 1, 1, 2, 2, 3, 3, 0};

        m_selectionRectangleMesh->Set2DPositions(positions, std::size(positions));
        m_selectionRectangleMesh->SetIndices(indices, std::size(indices));
    }

    ~SelectionRectangleRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        const SelectionRectangle* selectionRectangle =
            renderComponentManager->ReadWorldComponent<SelectionRectangle>();
        if (selectionRectangle->IsSelecting())
        {
            const Box2f selectionBox = selectionRectangle->GetCurrentSelection();
            const glm::vec2 rectangleCenter = selectionBox.Center();
            const glm::vec2 rectangleSize = selectionBox.Extents();

            const glm::vec3 position = {rectangleCenter.x, rectangleCenter.y, 0.f};
            const glm::vec3 scale = {rectangleSize.x / 2.f, rectangleSize.y / 2.f, 0.f};

            glm::mat4 modelMatrix = glm::mat4(1.f);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::scale(modelMatrix, scale);

            frameRenderer.AddUIMeshToRender(m_selectionRectangleMesh.get(), modelMatrix);
        }
    }

private:
    std::unique_ptr<Mesh> m_selectionRectangleMesh;
};

const int dummy = RegisterRenderUpdater<SelectionRectangleRenderUpdater>("SelectionRectangle");

} // namespace
