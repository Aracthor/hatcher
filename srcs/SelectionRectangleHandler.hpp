#pragma once

#include "hatcher/Maths/Box.hpp"

class SelectionRectangleHandler
{
public:
    void StartSelection(const glm::vec2& position);
    void MoveSelection(const glm::vec2& position);
    void EndSelection();

    bool IsSelecting() const { return m_isSelecting; }
    const hatcher::Box2f GetCurrentSelection() const { return m_currentRectangle; }

private:
    bool m_isSelecting = false;
    glm::vec2 m_selectionStart;
    hatcher::Box2f m_currentRectangle;
};
