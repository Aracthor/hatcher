#pragma once

#include <memory>

#include "hatcher/Maths/Box.hpp"

namespace hatcher
{
class IFrameRenderer;
class IRendering;
class Mesh;
} // namespace hatcher

using namespace hatcher;

class SelectionRectangleHandler
{
public:
    SelectionRectangleHandler(const IRendering* rendering);
    ~SelectionRectangleHandler();

    void StartSelection(const glm::vec2& position);
    void MoveSelection(const glm::vec2& position);
    void EndSelection();

    bool IsSelecting() const { return m_isSelecting; }
    const Box2f GetCurrentSelection() const { return m_currentRectangle; }
    void DrawSelectionRectangle(IFrameRenderer& frameRenderer) const;

private:
    bool m_isSelecting = false;
    glm::vec2 m_selectionStart;
    Box2f m_currentRectangle;
    std::unique_ptr<Mesh> m_selectionRectangleMesh;
};
