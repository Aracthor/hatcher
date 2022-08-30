#include "SelectionRectangle.hpp"

#include "hatcher/assert.hpp"

using namespace hatcher;

void SelectionRectangle::StartSelection(const glm::vec2& position)
{
    m_isSelecting = true;
    m_selectionStart = position;
    m_currentRectangle = Box2f(m_selectionStart);
}

void SelectionRectangle::MoveSelection(const glm::vec2& position)
{
    HATCHER_ASSERT(m_isSelecting);
    m_currentRectangle = Box2f(m_selectionStart);
    m_currentRectangle.AddPoint(position);
}

void SelectionRectangle::EndSelection()
{
    m_isSelecting = false;
}
