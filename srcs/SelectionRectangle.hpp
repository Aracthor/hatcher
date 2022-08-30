#pragma once

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/Maths/Box.hpp"

using namespace hatcher;

class SelectionRectangle final : public IWorldComponent
{
public:
    void StartSelection(const glm::vec2& position);
    void MoveSelection(const glm::vec2& position);
    void EndSelection();

    bool IsSelecting() const { return m_isSelecting; }
    const Box2f GetCurrentSelection() const { return m_currentRectangle; }

private:
    bool m_isSelecting = false;
    glm::vec2 m_selectionStart;
    Box2f m_currentRectangle;
};
