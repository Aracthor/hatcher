#include "FrameRenderer.hpp"

#include "Core/GLContext.hpp"

#include "Material.hpp"

namespace hatcher
{

FrameRenderer::FrameRenderer(GLContext* context, const Clock* clock, const glm::vec2& resolution)
    : m_context(context)
    , m_clock(clock)
    , m_resolution(resolution)
{
}

FrameRenderer::~FrameRenderer() = default;

void FrameRenderer::PrepareSceneDraw(const Material* material) const
{
    material->Use();
    material->SetTransformationMatrix("uniViewMatrix", m_viewMatrix);
    material->SetTransformationMatrix("uniProjectionMatrix", m_projectionMatrix);
}

void FrameRenderer::PrepareUIDraw(const Material* material) const
{
    material->Use();
    const float width = static_cast<float>(m_resolution.x);
    const float height = static_cast<float>(m_resolution.y);
    const glm::mat4 UIProjectionMatrix = glm::ortho(0.f, width, 0.f, height);
    material->SetTransformationMatrix("uniViewMatrix", glm::mat4(1.f));
    material->SetTransformationMatrix("uniProjectionMatrix", UIProjectionMatrix);
}

void FrameRenderer::SetProjectionMatrix(const glm::mat4& matrix)
{
    m_projectionMatrix = matrix;
}

void FrameRenderer::SetViewMatrix(const glm::mat4& matrix)
{
    m_viewMatrix = matrix;
}

void FrameRenderer::EnableDepthTest()
{
    m_context->EnableDepthTest();
}

void FrameRenderer::DisableDepthTest()
{
    m_context->DisableDepthTest();
}

glm::vec2 FrameRenderer::WorldCoordsToWindowCoords(const glm::vec3& worldCoords, const glm::mat4& modelMatrix) const
{
    const glm::ivec2 resolution = m_resolution;
    const glm::vec4 projectedVertex = m_projectionMatrix * m_viewMatrix * modelMatrix * glm::vec4(worldCoords, 1.f);
    return {(projectedVertex.x + 1.f) / 2.f * resolution.x, (projectedVertex.y + 1.f) / 2.f * resolution.y};
}

Box2f FrameRenderer::ProjectBox3DToWindowCoords(const Box3f& box, const glm::mat4& modelMatrix) const
{
    Box2f result = WorldCoordsToWindowCoords(box.Min(), modelMatrix);
    std::array<glm::vec3, 8> corners = box.GetCorners();

    for (const glm::vec3& corner : corners)
    {
        result.AddPoint(WorldCoordsToWindowCoords(corner, modelMatrix));
    }
    return result;
}

glm::vec3 FrameRenderer::WindowCoordsToWorldCoords(const glm::vec2 windowCoords) const
{
    const glm::ivec2 resolution = m_resolution;
    const glm::vec3 winCoords(windowCoords.x, windowCoords.y, 0.f);
    const glm::mat4 modelViewMatrix = m_viewMatrix;
    const glm::vec4 viewport = {0.f, 0.f, resolution.x, resolution.y};
    return glm::unProject(winCoords, modelViewMatrix, m_projectionMatrix, viewport);
}

} // namespace hatcher
