#include "Rendering.hpp"

#include <SDL2/SDL_events.h>

#include "Core/Window.hpp"

#include "hatcher/Clock.hpp"
#include "hatcher/IApplication.hpp"
#include "hatcher/World.hpp"
#include "hatcher/WorldManager.hpp"

#include "FrameRenderer.hpp"
#include "MaterialFactory.hpp"
#include "MeshLoader.hpp"

namespace hatcher
{

Rendering::Rendering(const char* name, int windowWidth, int windowHeight, const FileSystem* fileSystem)
{
    m_clock = make_unique<Clock>();
    m_materialFactory = make_unique<MaterialFactory>(fileSystem);
    m_meshLoader = make_unique<MeshLoader>(fileSystem);
    m_window = make_unique<Window>(name, windowWidth, windowHeight);
    m_frameRenderer = make_unique<FrameRenderer>(m_window->Context(), m_clock.get(), Resolution());
}

Rendering::~Rendering() = default;

glm::ivec2 Rendering::Resolution() const
{
    return {m_window->Width(), m_window->Height()};
}

void Rendering::HandleWindowEvents(IApplication* application, WorldManager* worldManager, World* world)
{
    const std::vector<SDL_Event> events = m_window->PollEvents();
    const auto IsStopEvent = [](const SDL_Event& event) { return event.type == SDL_QUIT; };
    if (std::find_if(events.begin(), events.end(), IsStopEvent) != events.end())
        application->Stop();
    worldManager->UpdateFromEvents(span<const SDL_Event>(events), application, world, *m_frameRenderer);
}

void Rendering::UpdateWorldRendering(IApplication* application, WorldManager* worldManager, World* world)
{
    m_window->Clear();
    m_clock->Update();

    m_frameRenderer->SetCurrentTick(world->CurrentTick());
    worldManager->UpdateRendering(application, world, *m_frameRenderer);
    m_window->Refresh();
}

} // namespace hatcher
