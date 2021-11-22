#pragma once

#include <memory>

#include "hatcher/Updater.hpp"

namespace hatcher
{
class ShaderProgram;
class VertexArrayObject;
class VertexBufferObject;
class World;
} // namespace hatcher

class SquareDisplayUpdater final : public hatcher::Updater
{
public:
    SquareDisplayUpdater();
    ~SquareDisplayUpdater();

    void Update(hatcher::ComponentManager* componentManager) override;

private:
    std::unique_ptr<hatcher::VertexBufferObject> m_vbo;
    std::unique_ptr<hatcher::VertexBufferObject> m_elements_vbo;
    std::unique_ptr<hatcher::VertexArrayObject> m_vao;
    std::unique_ptr<hatcher::ShaderProgram> m_program;
};
