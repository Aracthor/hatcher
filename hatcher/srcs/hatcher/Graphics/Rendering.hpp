#pragma once

#include "IRendering.hpp"
#include "hatcher/glm_pure.hpp"
#include "hatcher/unique_ptr.hpp"

namespace hatcher
{
class Clock;
class FrameRenderer;
class IApplication;
class MaterialFactory;
class MeshLoader;
class Window;
class World;

class Rendering : public IRendering
{
public:
    Rendering(const char* name, int windowWidth, int windowHeight);
    ~Rendering();

    void UpdateWorldRendering(IApplication* application, World* parWorld);
    void RenderWorld();

    const unique_ptr<MaterialFactory>& GetMaterialFactory() const override { return m_materialFactory; }

    const unique_ptr<MeshLoader>& GetMeshLoader() const override { return m_meshLoader; }

    const unique_ptr<Clock>& GetClock() const { return m_clock; }

private:
    glm::ivec2 Resolution() const;

    unique_ptr<Clock> m_clock;
    unique_ptr<MaterialFactory> m_materialFactory;
    unique_ptr<MeshLoader> m_meshLoader;
    unique_ptr<FrameRenderer> m_frameRenderer;
    unique_ptr<Window> m_window;
};

} // namespace hatcher
