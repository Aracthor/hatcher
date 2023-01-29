#pragma once

#include <memory>

#include "IRendering.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class Clock;
class FrameRenderer;
class IApplication;
class MaterialFactory;
class Window;
class World;

class Rendering : public IRendering
{
public:
    Rendering(const char* name, int windowWidth, int windowHeight);
    ~Rendering();

    void UpdateWorldRendering(IApplication* application, World* parWorld);
    void RenderWorld();

    const std::unique_ptr<MaterialFactory>& GetMaterialFactory() const override
    {
        return m_materialFactory;
    }

private:
    glm::ivec2 Resolution() const;

    std::unique_ptr<Clock> m_clock;
    std::unique_ptr<MaterialFactory> m_materialFactory;
    std::unique_ptr<FrameRenderer> m_frameRenderer;
    std::unique_ptr<Window> m_window;
};

} // namespace hatcher
