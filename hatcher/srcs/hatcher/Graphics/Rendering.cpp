#include "Rendering.hpp"

#include "Core/Window.hpp"

#include "hatcher/World.hpp"

#include "Clock.hpp"
#include "FrameRenderer.hpp"
#include "MaterialFactory.hpp"
#include "MeshBuilder.hpp"

namespace hatcher
{

Rendering::Rendering(const char* name, int windowWidth, int windowHeight)
{
    m_clock = std::make_unique<Clock>();
    m_materialFactory = std::make_unique<MaterialFactory>();
    m_meshBuilder = std::make_unique<MeshBuilder>();
    m_window = std::make_unique<Window>(name, windowWidth, windowHeight);
    m_frameRenderer = std::make_unique<FrameRenderer>(m_clock.get(), Resolution());
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
    world->UpdateRendering(application, *m_frameRenderer, *this);
}

void Rendering::RenderWorld()
{
    m_frameRenderer->Render();
    m_window->Refresh();
}

} // namespace hatcher
