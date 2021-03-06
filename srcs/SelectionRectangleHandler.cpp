#include "SelectionRectangleHandler.hpp"

#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/assert.hpp"

SelectionRectangleHandler::SelectionRectangleHandler(const hatcher::IRendering* rendering)
{
    hatcher::MeshBuilder* meshBuilder = rendering->GetMeshBuilder().get();
    meshBuilder->SetPrimitive(hatcher::Primitive::Lines);

    meshBuilder->SetMaterial(rendering->GetMaterialFactory()->CreateMaterial(
        "shaders/hello_world_2D.vert", "shaders/hello_world.frag"));
    m_selectionRectangleMesh.reset(meshBuilder->Create());

    float positions[] = {
        -1.f, -1.f,

        1.f,  -1.f,

        1.f,  1.f,

        -1.f, 1.f,
    };
    hatcher::ushort indices[] = {0, 1, 1, 2, 2, 3, 3, 0};

    m_selectionRectangleMesh->Set2DPositions(positions, std::size(positions));
    m_selectionRectangleMesh->SetIndices(indices, std::size(indices));
}

SelectionRectangleHandler::~SelectionRectangleHandler() = default;

void SelectionRectangleHandler::StartSelection(const glm::vec2& position)
{
    m_isSelecting = true;
    m_selectionStart = position;
    m_currentRectangle = hatcher::Box2f(m_selectionStart);
}

void SelectionRectangleHandler::MoveSelection(const glm::vec2& position)
{
    HATCHER_ASSERT(m_isSelecting);
    m_currentRectangle = hatcher::Box2f(m_selectionStart);
    m_currentRectangle.AddPoint(position);
}

void SelectionRectangleHandler::EndSelection()
{
    m_isSelecting = false;
}

void SelectionRectangleHandler::DrawSelectionRectangle(hatcher::IFrameRenderer& frameRenderer) const
{
    if (IsSelecting())
    {
        const glm::vec2 rectangleCenter = m_currentRectangle.Center();
        const glm::vec2 rectangleSize = m_currentRectangle.Extents();

        const glm::vec3 position = {rectangleCenter.x, rectangleCenter.y, 0.f};
        const glm::vec3 scale = {rectangleSize.x / 2.f, rectangleSize.y / 2.f, 0.f};

        glm::mat4 modelMatrix = glm::mat4(1.f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, scale);

        frameRenderer.AddUIMeshToRender(m_selectionRectangleMesh.get(), modelMatrix);
    }
}
