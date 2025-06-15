#include <sstream>

#include "RenderUpdaterOrder.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/NameComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"

#include "imgui.h"

namespace
{
bool enabled = false;

class InventoryPanelEventListener : public IEventListener
{
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_I)
        {
            enabled = !enabled;
        }
    }
};

class InventoryPanelRenderUpdater : public RenderUpdater
{
public:
    InventoryPanelRenderUpdater(const IRendering* rendering) {}

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        if (!enabled)
            return;

        const auto inventoryComponents = componentManager->ReadComponents<InventoryComponent>();
        const auto selectableComponents = renderComponentManager->ReadComponents<SelectableComponent>();
        const auto nameComponents = componentManager->ReadComponents<NameComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (selectableComponents[i] && selectableComponents[i]->selected && inventoryComponents[i])
            {
                HATCHER_ASSERT(nameComponents[i]);
                const InventoryComponent& inventory = *inventoryComponents[i];
                ImGui::PushID(i);
                ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);
                std::ostringstream windowNameOss;
                windowNameOss << "Inventory - " << nameComponents[i]->name << "##" << i;
                if (ImGui::Begin(windowNameOss.str().c_str(), &enabled))
                {
                    ImGui::Text("Storage: %ld", inventory.storage.size());
                    for (Entity::IDType itemID : inventory.storage)
                    {
                        HATCHER_ASSERT(nameComponents[itemID]);
                        ImGui::Selectable(nameComponents[itemID]->name.c_str());
                    }
                }
                ImGui::End();
                ImGui::PopID();
            }
        }
    }
};

EventListenerRegisterer<InventoryPanelEventListener> eventRegisterer;
RenderUpdaterRegisterer<InventoryPanelRenderUpdater> updaterRegisterer((int)ERenderUpdaterOrder::Interface);

} // namespace
