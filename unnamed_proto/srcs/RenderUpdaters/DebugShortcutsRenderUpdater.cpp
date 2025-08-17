#include "RenderUpdaterOrder.hpp"

#include "hatcher/CommandRegisterer.hpp"
#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/assert.hpp"

#include "RenderComponents/SelectableComponent.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{

class DeleteEntityCommand final : public ICommand
{
public:
    DeleteEntityCommand(Entity entity)
        : m_entity(entity)
    {
    }

    void Save(DataSaver& saver) const override { saver << m_entity; }
    void Load(DataLoader& loader) override { loader >> m_entity; }

    void Execute(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        entityManager->DeleteEntity(m_entity);
    }

private:
    Entity m_entity;

    COMMAND_HEADER(DeleteEntityCommand)
};
REGISTER_COMMAND(DeleteEntityCommand);

class DebugShortcutsEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DELETE &&
            event.key.keysym.mod & KMOD_ALT)
        {
            const auto selectableComponents = renderComponentAccessor->ReadComponents<SelectableComponent>();

            for (int i = 0; i < renderComponentAccessor->Count(); i++)
            {
                if (selectableComponents[i] && selectableComponents[i]->selected)
                {
                    // TODO find a better way to convert an int to Entity ?
                    commandManager->AddCommand(new DeleteEntityCommand(Entity(i)));
                }
            }
        }
    }
};

EventListenerRegisterer<DebugShortcutsEventListener> registerer;

} // namespace
