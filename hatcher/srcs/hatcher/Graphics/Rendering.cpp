#include "Rendering.hpp"

#include "Core/Window.hpp"

#include "hatcher/Threading/Mutex.hpp"
#include "hatcher/World.hpp"

#include "Clock.hpp"
#include "FrameRenderer.hpp"
#include "MeshBuilder.hpp"

namespace hatcher
{

Rendering::Rendering(const char* name, int windowWidth, int windowHeight)
{
    m_mutex = std::make_unique<Mutex>();

    m_clock = std::make_unique<Clock>();
    m_meshBuilder = std::make_unique<MeshBuilder>();
    m_window = std::make_unique<Window>(name, windowWidth, windowHeight);
}

Rendering::~Rendering() = default;

void Rendering::UpdateWorldRendering(World* world)
{
    // TODO clear instead ?
    std::shared_ptr<FrameRenderer> frameRenderer = std::make_shared<FrameRenderer>();

    m_clock->Update();
    world->UpdateRendering(*frameRenderer, *m_clock);

    m_mutex->Lock();
    m_frameRenderer = frameRenderer;
    m_mutex->Unlock();
}

void Rendering::RenderWorld()
{
    m_mutex->Lock();
    std::shared_ptr<FrameRenderer> frameRenderer = m_frameRenderer;
    m_mutex->Unlock();

    if (frameRenderer)
    {
        m_window->Clear();
        frameRenderer->Render();
        m_window->Refresh();
    }
}

} // namespace hatcher
