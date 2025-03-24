#include <iostream>

#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"
#include "hatcher/EntityManager.hpp"
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
    SaveCommand(std::vector<ubyte>& save)
        : m_save(save)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        DataSaver saver;
        entityManager->Save(saver);
        m_save = saver.Result();
        std::cout << "State saved." << std::endl;
    }

private:
    std::vector<ubyte>& m_save;
};

class LoadCommand final : public ICommand
{
public:
    LoadCommand(std::vector<ubyte>& save)
        : m_save(save)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        if (!m_save.empty())
        {
            DataLoader loader(m_save);
            entityManager->Load(loader);
            std::cout << "State loaded." << std::endl;
        }
        else
        {
            std::cout << "No state to load." << std::endl;
        }
    }

private:
    std::vector<ubyte>& m_save;
};

// TODO make this updater part of core hatcher instead of specific to each application ?
class SaveLoaderRenderUpdater final : public RenderUpdater
{
public:
    SaveLoaderRenderUpdater(const IRendering* rendering) {}

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
    }

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

private:
    std::vector<ubyte> m_save;
};

RenderUpdaterRegisterer<SaveLoaderRenderUpdater> registerer;

} // namespace
