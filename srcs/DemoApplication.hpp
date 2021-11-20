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
    void HandleEvents() override;
};