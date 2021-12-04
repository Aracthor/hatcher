#include "SelectionRectangleHandler.hpp"

#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/assert.hpp"

SelectionRectangleHandler::SelectionRectangleHandler(
    const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
{
    meshBuilder->SetPrimitive(hatcher::Primitive::Lines);
    meshBuilder->SetDynamic();

    meshBuilder->SetProgram("shaders/hello_world.vert", "shaders/hello_world.frag");
    m_selectionRectangleMesh.reset(meshBuilder->Create());

    hatcher::ushort indices[] = {0, 1, 1, 2, 2, 3, 3, 0};

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
        // clang-format off
        float rectangleCorners[] =
        {
            m_currentRectangle.Min().x, m_currentRectangle.Min().y,
            m_currentRectangle.Max().x, m_currentRectangle.Min().y,
            m_currentRectangle.Max().x, m_currentRectangle.Max().y,
            m_currentRectangle.Min().x, m_currentRectangle.Max().y,
        };
        // clang-format on

        m_selectionRectangleMesh->SetPositions(rectangleCorners, std::size(rectangleCorners));

        const glm::mat4 identityMatrix(1.f);
        frameRenderer.AddMeshToRender(m_selectionRectangleMesh.get(), identityMatrix);
    }
}
