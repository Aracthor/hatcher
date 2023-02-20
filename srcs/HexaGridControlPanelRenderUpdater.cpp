#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "imgui.h"

#include "Camera.hpp"
#include "HexagonalGrid.hpp"

namespace
{
struct ControlPanel
{
    bool enabled = false;
    bool walkable = false;
};

class HexaGridControlPanelEventListener final : public IEventListener
{
public:
    HexaGridControlPanelEventListener(ControlPanel& panel)
        : m_panel(panel)
    {
    }

    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_F2)
            {
                m_panel.enabled = !m_panel.enabled;
            }
        }
        else if (m_panel.enabled)
        {
            HATCHER_ASSERT(event.type == SDL_MOUSEBUTTONDOWN);
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
                const glm::vec2 worldCoords2D =
                    camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
                HexagonalGrid* hexaGrid = componentManager->WriteWorldComponent<HexagonalGrid>();
                hexaGrid->SetTileWalkable(hexaGrid->PositionToTileCoords(worldCoords2D),
                                          m_panel.walkable);
            }
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
            SDL_MOUSEBUTTONDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }

private:
    ControlPanel& m_panel;
};

class HexaGridControlPanelRenderUpdater final : public RenderUpdater
{
public:
    HexaGridControlPanelRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(
            std::make_shared<HexaGridControlPanelEventListener>(m_panel));
    }

    ~HexaGridControlPanelRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        if (!m_panel.enabled)
            return;

        ImGui::SetNextWindowSize({200, 100}, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("HexaGrid Control Panel", &m_panel.enabled))
        {
            ImGui::Checkbox("Walkable", &m_panel.walkable);
        }
        ImGui::End();
    }

    ControlPanel m_panel;
};

const int dummy = RegisterRenderUpdater<HexaGridControlPanelRenderUpdater>("HexaGridControlPanel");

} // namespace
