#include "Rendering.hpp"

#include "Core/Window.hpp"

#include "hatcher/World.hpp"

#include "FrameRenderer.hpp"
#include "MeshBuilder.hpp"

namespace hatcher
{

Rendering::Rendering(const char* name, int windowWidth, int windowHeight)
{
    m_window = std::make_unique<Window>(name, windowWidth, windowHeight);
    m_meshBuilder = std::make_unique<MeshBuilder>();
}

Rendering::~Rendering() = default;

void Rendering::RenderWorld(World* world)
{
    FrameRenderer frameRenderer;

    world->UpdateRendering(frameRenderer);

    m_window->Clear();
    frameRenderer.Render();
    m_window->Refresh();
}

} // namespace hatcher
