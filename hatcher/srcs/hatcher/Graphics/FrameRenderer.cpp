#include "FrameRenderer.hpp"

#include "Mesh.hpp"

namespace hatcher
{

void FrameRenderer::AddMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix)
{
    m_meshesToRender.push_back({mesh, modelMatrix});
}

void FrameRenderer::SetProjectionMatrix(const glm::mat4& matrix)
{
    m_projectionMatrix = matrix;
}

void FrameRenderer::SetViewMatrix(const glm::mat4& matrix)
{
    m_viewMatrix = matrix;
}

void FrameRenderer::Render() const
{
    for (const MeshToRender& meshToRender : m_meshesToRender)
    {
        meshToRender.mesh->Draw(m_projectionMatrix, m_viewMatrix, meshToRender.modelMatrix);
    }
}

} // namespace hatcher
