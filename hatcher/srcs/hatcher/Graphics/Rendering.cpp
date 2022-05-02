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
}

Rendering::~Rendering() = default;

glm::ivec2 Rendering::Resolution() const
{
    return {m_window->Width(), m_window->Height()};
}

glm::vec2 Rendering::WorldCoordsToWindowCoords(const glm::vec3& worldCoords,
                                               const glm::mat4& modelMatrix) const
{
    const glm::ivec2 resolution = Resolution();
    const glm::vec4 projectedVertex =
        m_projectionMatrix * m_viewMatrix * modelMatrix * glm::vec4(worldCoords, 1.f);
    return {(projectedVertex.x + 1.f) / 2.f * resolution.x,
            (projectedVertex.y + 1.f) / 2.f * resolution.y};
}

hatcher::Box2f Rendering::ProjectBox3DToWindowCoords(const hatcher::Box3f& box,
                                                     const glm::mat4& modelMatrix) const
{
    hatcher::Box2f result = WorldCoordsToWindowCoords(box.Min(), modelMatrix);
    std::array<glm::vec3, 8> corners = box.GetCorners();

    for (const glm::vec3& corner : corners)
    {
        result.AddPoint(WorldCoordsToWindowCoords(corner, modelMatrix));
    }
    return result;
}

glm::vec3 Rendering::WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const
{
    const glm::ivec2 resolution = Resolution();
    const glm::vec3 winCoords(windowCoords.x, resolution.y - windowCoords.y, 0.f);
    const glm::mat4 modelViewMatrix = m_viewMatrix;
    const glm::vec4 viewport = {0.f, 0.f, resolution.x, resolution.y};
    return glm::unProject(winCoords, modelViewMatrix, m_projectionMatrix, viewport);
}

void Rendering::UpdateWorldRendering(World* world)
{
    // TODO clear instead ?
    m_frameRenderer = std::make_unique<FrameRenderer>();

    m_clock->Update();
    world->UpdateRendering(*m_frameRenderer, *this);

    m_projectionMatrix = m_frameRenderer->ProjectionMatrix();
    m_viewMatrix = m_frameRenderer->ViewMatrix();
}

void Rendering::RenderWorld()
{
    m_window->Clear();
    m_frameRenderer->Render(Resolution());
    m_window->Refresh();
}

} // namespace hatcher
