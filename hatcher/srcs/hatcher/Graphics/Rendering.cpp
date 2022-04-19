#include "Rendering.hpp"

#include "Core/Window.hpp"

#include "hatcher/World.hpp"

#include "Clock.hpp"
#include "FrameRenderer.hpp"
#include "MeshBuilder.hpp"

namespace hatcher
{

Rendering::Rendering(const char* name, int windowWidth, int windowHeight)
{
    m_clock = std::make_unique<Clock>();
    m_meshBuilder = std::make_unique<MeshBuilder>();
    m_window = std::make_unique<Window>(name, windowWidth, windowHeight);
}

Rendering::~Rendering() = default;

glm::ivec2 Rendering::Resolution() const
{
    return {m_window->Width(), m_window->Height()};
}

void Rendering::UpdateWorldRendering(World* world)
{
    // TODO clear instead ?
    m_frameRenderer = std::make_unique<FrameRenderer>();

    m_clock->Update();
    world->UpdateRendering(*m_frameRenderer, *this, *m_clock);
}

void Rendering::RenderWorld()
{
    m_window->Clear();
    m_frameRenderer->Render();
    m_window->Refresh();
}

} // namespace hatcher
