#include "FrameRenderer.hpp"

#include "Mesh.hpp"

namespace hatcher
{

void FrameRenderer::AddMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix)
{
    m_meshesToRender.push_back({mesh, modelMatrix});
}

void FrameRenderer::AddUIMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix)
{
    m_UImeshesToRender.push_back({mesh, modelMatrix});
}

void FrameRenderer::SetProjectionMatrix(const glm::mat4& matrix)
{
    m_projectionMatrix = matrix;
}

void FrameRenderer::SetViewMatrix(const glm::mat4& matrix)
{
    m_viewMatrix = matrix;
}

void FrameRenderer::Render(const glm::vec2& resolution) const
{
    for (const MeshToRender& meshToRender : m_meshesToRender)
    {
        meshToRender.mesh->Draw(meshToRender.modelMatrix, m_viewMatrix, m_projectionMatrix);
    }

    const glm::mat4 UIProjectionMatrix = glm::ortho(0.f, resolution.x, 0.f, resolution.y);
    for (const MeshToRender& meshToRender : m_UImeshesToRender)
    {
        meshToRender.mesh->Draw(meshToRender.modelMatrix, glm::mat4(1.f), UIProjectionMatrix);
    }
}

} // namespace hatcher
