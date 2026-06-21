#include "FrameRenderer.hpp"

#include "Core/GLContext.hpp"

#include "Material.hpp"

namespace hatcher
{

FrameRenderer::FrameRenderer(GLContext* context, const Clock* clock, Vect2i resolution)
    : m_context(context)
    , m_clock(clock)
    , m_resolution(resolution)
{
}

FrameRenderer::~FrameRenderer() = default;

void FrameRenderer::PrepareSceneDraw(const Material* material) const
{
    material->Use();
    material->SetTransformationMatrix("uniProjectionViewMatrix", m_projectionMatrix * m_viewMatrix);
}

void FrameRenderer::PrepareUIDraw(const Material* material) const
{
    material->Use();
    const float width = static_cast<float>(m_resolution.x);
    const float height = static_cast<float>(m_resolution.y);
    const Mat4f UIProjectionMatrix = Mat4f::Orthographic(0.f, width, 0.f, height);
    material->SetTransformationMatrix("uniProjectionViewMatrix", UIProjectionMatrix);
}

void FrameRenderer::SetProjectionMatrix(const Mat4f& matrix)
{
    m_projectionMatrix = matrix;
}

void FrameRenderer::SetViewMatrix(const Mat4f& matrix)
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

Vect2f FrameRenderer::WorldCoordsToWindowCoords(Vect3f worldCoords, const Mat4f& modelMatrix) const
{
    const Vect2i resolution = m_resolution;
    const Vect4f projectedVertex = m_projectionMatrix * m_viewMatrix * modelMatrix * Vect4f(worldCoords, 1.f);
    return {(projectedVertex.x + 1.f) / 2.f * resolution.x, (projectedVertex.y + 1.f) / 2.f * resolution.y};
}

Box2f FrameRenderer::ProjectBox3DToWindowCoords(const Box3f& box, const Mat4f& modelMatrix) const
{
    Box2f result = WorldCoordsToWindowCoords(box.Min(), modelMatrix);
    std::array<Vect3f, 8> corners = box.GetCorners();

    for (const Vect3f& corner : corners)
    {
        result.AddPoint(WorldCoordsToWindowCoords(corner, modelMatrix));
    }
    return result;
}

Vect3f FrameRenderer::WindowCoordsToWorldCoords(Vect2f windowCoords) const
{
    const Vect2f resolution = m_resolution;
    const Vect3f winCoords(windowCoords.x, windowCoords.y, 0.f);
    const Mat4f modelViewMatrix = m_viewMatrix;
    const Vect4f viewport = {0.f, 0.f, resolution.x, resolution.y};
    return Mat4f::Unproject(winCoords, modelViewMatrix, m_projectionMatrix, viewport);
}

} // namespace hatcher
