#pragma once

#include <memory>

#include "hatcher/Maths/Box.hpp"

namespace hatcher
{
class IFrameRenderer;
class Mesh;
class MeshBuilder;
} // namespace hatcher

class SelectionRectangleHandler
{
public:
    SelectionRectangleHandler(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder,
                              const glm::vec2& windowResolution);
    ~SelectionRectangleHandler();

    void StartSelection(const glm::vec2& position);
    void MoveSelection(const glm::vec2& position);
    void EndSelection();

    bool IsSelecting() const { return m_isSelecting; }
    const hatcher::Box2f GetCurrentSelection() const { return m_currentRectangle; }
    void DrawSelectionRectangle(hatcher::IFrameRenderer& frameRenderer) const;

private:
    glm::vec2 m_windowResolution;

    bool m_isSelecting = false;
    glm::vec2 m_selectionStart;
    hatcher::Box2f m_currentRectangle;
    std::unique_ptr<hatcher::Mesh> m_selectionRectangleMesh;
};
