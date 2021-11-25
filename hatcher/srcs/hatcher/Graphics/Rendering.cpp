#include "Rendering.hpp"

#include "Core/Window.hpp"

#include "hatcher/World.hpp"

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
    m_window->Clear();
    world->UpdateRendering();
    m_window->Refresh();
}

} // namespace hatcher
