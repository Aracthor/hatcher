#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{

class DemoImguiEventListener final : public IEventListener
{
public:
    DemoImguiEventListener(bool& showDemo)
        : m_showDemo(showDemo)
    {
    }

    void GetEvent(const SDL_Event& event, IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_D && event.key.keysym.mod & KMOD_CTRL)
        {
            m_showDemo = !m_showDemo;
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }

private:
    bool& m_showDemo;
};

class DemoImguiRenderUpdater final : public RenderUpdater
{
public:
    DemoImguiRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<DemoImguiEventListener>(m_showDemo));
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        if (m_showDemo)
            ImGui::ShowDemoWindow(&m_showDemo);
    }

private:
    bool m_showDemo = false;
};

const int dummy = RegisterRenderUpdater<DemoImguiRenderUpdater>();

} // namespace