#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "InventoryComponent.hpp"
#include "ItemComponent.hpp"
#include "SelectableComponent.hpp"

#include "imgui.h"

namespace
{

class InventoryPanelRenderUpdater final : public RenderUpdater
{
public:
    InventoryPanelRenderUpdater(const IRendering* rendering) {}

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        if (!m_enabled)
            return;

        const auto inventoryComponents = componentManager->ReadComponents<InventoryComponent>();
        const auto itemComponents = componentManager->ReadComponents<ItemComponent>();
        const auto selectableComponents = renderComponentManager->ReadComponents<SelectableComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (selectableComponents[i] && selectableComponents[i]->selected && inventoryComponents[i])
            {
                const InventoryComponent& inventory = *inventoryComponents[i];
                ImGui::PushID(i);
                ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once);
                if (ImGui::Begin("Inventory"), &m_enabled)
                {
                    ImGui::Text("Storage: %ld", inventory.storage.size());
                    for (Entity::IDType itemID : inventory.storage)
                    {
                        HATCHER_ASSERT(itemComponents[itemID]);
                        ImGui::Selectable(itemComponents[itemID]->name.c_str());
                    }
                }
                ImGui::End();
                ImGui::PopID();
            }
        }
    }

    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_I)
        {
            m_enabled = !m_enabled;
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }

    bool m_enabled = false;
};

RenderUpdaterRegisterer<InventoryPanelRenderUpdater> registerer;

} // namespace
