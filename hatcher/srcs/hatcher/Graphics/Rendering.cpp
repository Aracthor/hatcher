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

void Rendering::RenderWorld(World* world)
{
    FrameRenderer frameRenderer;

    m_clock->Update();
    world->UpdateRendering(frameRenderer, *m_clock);

    m_window->Clear();
    frameRenderer.Render();
    m_window->Refresh();
}

} // namespace hatcher
