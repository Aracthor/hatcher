#pragma once

#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class Mesh;

class IFrameRenderer
{
public:
    virtual ~IFrameRenderer() = default;

    virtual void AddMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix) = 0;
    virtual void AddUIMeshToRender(const Mesh* mesh, const glm::mat4& modelMatrix) = 0;
    virtual void SetProjectionMatrix(const glm::mat4& matrix) = 0;
    virtual void SetViewMatrix(const glm::mat4& matrix) = 0;
};

} // namespace hatcher
