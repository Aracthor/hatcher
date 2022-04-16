#pragma once

#include <utility>
#include <vector>

#include "IFrameRenderer.hpp"

#include "hatcher/glm_pure.hpp"

namespace hatcher
{

class FrameRenderer : public IFrameRenderer
{
public:
    void AddMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix) override;
    void SetProjectionMatrix(const glm::mat4& matrix) override;
    void SetViewMatrix(const glm::mat4& matrix) override;

    void Render() const;

private:
    using MeshToRender = std::pair<const Mesh*, glm::mat4>;

    std::vector<MeshToRender> m_meshesToRender;
    glm::mat4 m_viewMatrix = glm::mat4(1.f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
};

} // namespace hatcher
