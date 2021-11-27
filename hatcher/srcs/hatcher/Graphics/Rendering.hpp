#pragma once

#include <memory>

#include "IRendering.hpp"

namespace hatcher
{
class Clock;
class MeshBuilder;
class Window;
class World;

class Rendering : public IRendering
{
public:
    Rendering(const char* name, int windowWidth, int windowHeight);
    ~Rendering();

    void RenderWorld(World* parWorld) override;

    const std::unique_ptr<MeshBuilder>& GetMeshBuilder() override { return m_meshBuilder; }

private:
    std::unique_ptr<Clock> m_clock;
    std::unique_ptr<MeshBuilder> m_meshBuilder;
    std::unique_ptr<Window> m_window;
};

} // namespace hatcher
