#include "hatcher/CommandRegisterer.hpp"
#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/assert.hpp"

#include "imgui.h"

#include "WorldComponents/Camera.hpp"
#include "WorldComponents/HexagonalGrid.hpp"

namespace
{
struct ControlPanel
{
    bool enabled = false;
    bool walkable = false;
};

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

class HexaGridControlPanelRenderUpdater final : public RenderUpdater
{
public:
    HexaGridControlPanelRenderUpdater(const IRendering* rendering) {}

    ~HexaGridControlPanelRenderUpdater() = default;

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
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

    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_F2)
            {
                m_panel.enabled = !m_panel.enabled;
            }
        }
        else if (event.type && m_panel.enabled)
        {
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
                const glm::vec2 worldCoords2D =
                    camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
                commandManager->AddCommand(new SetTileWaklableCommand(worldCoords2D, m_panel.walkable));
            }
        }
    }

    ControlPanel m_panel;
};

RenderUpdaterRegisterer<HexaGridControlPanelRenderUpdater> registerer;

} // namespace
