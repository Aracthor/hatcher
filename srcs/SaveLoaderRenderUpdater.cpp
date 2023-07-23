#include <iostream>
#include <string>

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentSaver.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{

class SaveCommand final : public ICommand
{
public:
    SaveCommand(std::string& save)
        : m_save(save)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        ComponentSaver saver;
        entityManager->Save(saver);
        m_save = saver.Result();
        std::cout << "State saved." << std::endl;
    }

private:
    std::string& m_save;
};

class LoadCommand final : public ICommand
{
public:
    LoadCommand(std::string& save)
        : m_save(save)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        if (!m_save.empty())
        {
            ComponentLoader loader(m_save);
            entityManager->Load(loader);
            std::cout << "State loaded." << std::endl;
        }
        else
        {
            std::cout << "No state to load." << std::endl;
        }
    }

private:
    std::string& m_save;
};

// TODO make this updater part of core hatcher instead of specific to each application ?
class SaveLoaderEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_F5)
        {
            commandManager->AddCommand(new SaveCommand(m_save));
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_F9)
        {
            commandManager->AddCommand(new LoadCommand(m_save));
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }

    std::string m_save;
};

class SaveLoaderRenderUpdater final : public RenderUpdater
{
public:
    SaveLoaderRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<SaveLoaderEventListener>());
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
    }
};

RenderUpdaterRegisterer<SaveLoaderRenderUpdater> registerer;

} // namespace
