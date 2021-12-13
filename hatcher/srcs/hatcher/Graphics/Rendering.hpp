#pragma once

#include <memory>

#include "IRendering.hpp"

namespace hatcher
{
class Clock;
class FrameRenderer;
class MeshBuilder;
class Mutex;
class Window;
class World;

class Rendering : public IRendering
{
public:
    Rendering(const char* name, int windowWidth, int windowHeight);
    ~Rendering();

    void UpdateWorldRendering(World* parWorld) override;
    void RenderWorld() override;

    const std::unique_ptr<MeshBuilder>& GetMeshBuilder() override { return m_meshBuilder; }

private:
    std::unique_ptr<Mutex> m_mutex;

    std::unique_ptr<Clock> m_clock;
    std::unique_ptr<MeshBuilder> m_meshBuilder;
    std::shared_ptr<FrameRenderer> m_frameRenderer;
    std::unique_ptr<Window> m_window;
};

} // namespace hatcher
