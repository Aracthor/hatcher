#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{

class DemoImguiRenderUpdater final : public RenderUpdater
{
public:
    DemoImguiRenderUpdater(const IRendering* rendering) {}

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        if (m_showDemo)
            ImGui::ShowDemoWindow(&m_showDemo);
    }

    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_D &&
            event.key.keysym.mod & KMOD_CTRL)
        {
            m_showDemo = !m_showDemo;
        }
    }

private:
    bool m_showDemo = false;
};

RenderUpdaterRegisterer<DemoImguiRenderUpdater> registerer;

} // namespace