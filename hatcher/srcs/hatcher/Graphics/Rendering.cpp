#include "Rendering.hpp"

#include <SDL2/SDL_events.h>

#include "Core/Window.hpp"

#include "hatcher/World.hpp"

#include "Clock.hpp"
#include "FrameRenderer.hpp"
#include "MaterialFactory.hpp"
#include "MeshLoader.hpp"

namespace hatcher
{

Rendering::Rendering(const char* name, int windowWidth, int windowHeight)
{
    m_clock = make_unique<Clock>();
    m_materialFactory = make_unique<MaterialFactory>();
    m_meshLoader = make_unique<MeshLoader>();
    m_window = make_unique<Window>(name, windowWidth, windowHeight);
    m_frameRenderer = make_unique<FrameRenderer>(m_clock.get(), Resolution());
}

Rendering::~Rendering() = default;

glm::ivec2 Rendering::Resolution() const
{
    return {m_window->Width(), m_window->Height()};
}

void Rendering::UpdateWorldRendering(IApplication* application, World* world)
{
    m_frameRenderer->Clear();
    m_window->Clear();
    m_clock->Update();

    const std::vector<SDL_Event> events = m_window->PollEvents();
    world->UpdateFromEvents(span<const SDL_Event>(events), application, *m_frameRenderer);
    world->UpdateRendering(*m_frameRenderer);
}

void Rendering::RenderWorld()
{
    m_frameRenderer->RenderScene();
    m_window->DisableDepthTest();
    m_frameRenderer->RenderUI();
    m_window->EnableDepthTest();
    m_window->Refresh();
}

} // namespace hatcher
