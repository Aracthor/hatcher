#include "RenderUpdaterOrder.hpp"

#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{
bool showDemo = false;

class DemoImguiEventListener : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_D &&
            event.key.keysym.mod & KMOD_CTRL)
        {
            showDemo = !showDemo;
        }
    }
};

class DemoImguiRenderUpdater : public RenderUpdater
{
public:
    DemoImguiRenderUpdater(const IRendering* rendering) {}

    void Update(IApplication* application, const ComponentAccessor* componentAccessor,
                ComponentAccessor* renderComponentAccessor, IFrameRenderer& frameRenderer) override
    {
        if (showDemo)
            ImGui::ShowDemoWindow(&showDemo);
    }
};

EventListenerRegisterer<DemoImguiEventListener> eventRegisterer;
RenderUpdaterRegisterer<DemoImguiRenderUpdater> updaterRegisterer((int)ERenderUpdaterOrder::Interface);

} // namespace