#pragma once

#include <memory>

#include "hatcher/GameApplication.hpp"

namespace hatcher
{
class ShaderProgram;
class VertexArrayObject;
class VertexBufferObject;
} // namespace hatcher

class DemoApplication final : public hatcher::GameApplication
{
public:
    DemoApplication();
    ~DemoApplication();

private:
    std::unique_ptr<hatcher::VertexBufferObject> m_vbo;
    std::unique_ptr<hatcher::VertexArrayObject> m_vao;
    std::unique_ptr<hatcher::ShaderProgram> m_program;

    void HandleEvents() override;
    void RenderUpdate() override;
};