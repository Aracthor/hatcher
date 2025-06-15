#include "RenderUpdaterOrder.hpp"

#include "hatcher/CommandRegisterer.hpp"
#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/assert.hpp"

#include "imgui.h"

#include "WorldComponents/Camera.hpp"
#include "WorldComponents/HexagonalGrid.hpp"

namespace
{
struct
{
    bool enabled = false;
    bool walkable = false;
} controlPanel;

class SetTileWaklableCommand final : public ICommand
{
public:
    SetTileWaklableCommand(glm::vec2 worldCoords2D, bool walkable)
        : m_worldCoords2D(worldCoords2D)
        , m_walkable(walkable)
    {
    }

    void Save(DataSaver& saver) const override
    {
        saver << m_worldCoords2D;
        saver << m_walkable;
    }

    void Load(DataLoader& loader) override
    {
        loader >> m_worldCoords2D;
        loader >> m_walkable;
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        HexagonalGrid* hexaGrid = componentManager->WriteWorldComponent<HexagonalGrid>();
        hexaGrid->SetTileWalkable(hexaGrid->PositionToTileCoords(m_worldCoords2D), m_walkable);
    }

private:
    glm::vec2 m_worldCoords2D;
    bool m_walkable;

    COMMAND_HEADER(SetTileWaklableCommand)
};
REGISTER_COMMAND(SetTileWaklableCommand);

class HexaGridControlPanelEventListener : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_F2)
            {
                controlPanel.enabled = !controlPanel.enabled;
            }
        }
        else if (event.type && controlPanel.enabled)
        {
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
                const glm::vec2 worldCoords2D =
                    camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
                commandManager->AddCommand(new SetTileWaklableCommand(worldCoords2D, controlPanel.walkable));
            }
        }
    }
};

class HexaGridControlPanelRenderUpdater : public RenderUpdater
{
public:
    HexaGridControlPanelRenderUpdater(const IRendering* rendering) {}

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        if (!controlPanel.enabled)
            return;

        ImGui::SetNextWindowSize({200, 100}, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("HexaGrid Control Panel", &controlPanel.enabled))
        {
            ImGui::Checkbox("Walkable", &controlPanel.walkable);
        }
        ImGui::End();
    }
};

EventListenerRegisterer<HexaGridControlPanelEventListener> eventRegisterer;
RenderUpdaterRegisterer<HexaGridControlPanelRenderUpdater> updaterRegisterer((int)ERenderUpdaterOrder::Scene);

} // namespace
