#include "SelectionRectangleHandler.hpp"

#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/assert.hpp"

SelectionRectangleHandler::SelectionRectangleHandler(
    const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
{
    meshBuilder->SetPrimitive(hatcher::Primitive::Lines);

    meshBuilder->SetMaterial(
        meshBuilder->CreateMaterial("shaders/hello_world_2D.vert", "shaders/hello_world.frag"));
    m_selectionRectangleMesh.reset(meshBuilder->Create());

    float positions[] = {
        -0.5f, -0.5f,

        0.5f,  -0.5f,

        0.5f,  0.5f,

        -0.5f, 0.5f,
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

void SelectionRectangleHandler::DrawSelectionRectangle(hatcher::IFrameRenderer& frameRenderer)
{
    if (IsSelecting())
    {
        const glm::vec2 rectangleSize = m_currentRectangle.Extents();
        const glm::vec2 center = m_currentRectangle.Center();
        glm::mat4 modelMatrix = glm::mat4(1.f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(center.x, center.y, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(rectangleSize.x, rectangleSize.y, 1.f));

        frameRenderer.AddMeshToRender(m_selectionRectangleMesh.get(), modelMatrix);
    }
}
