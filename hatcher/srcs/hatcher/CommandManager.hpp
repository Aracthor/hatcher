#pragma once

#include <vector>

#include "ICommand.hpp"
#include "ICommandManager.hpp"
#include "unique_ptr.hpp"

namespace hatcher
{
class ComponentManager;
class IEntityManager;

class CommandManager final : public ICommandManager
{
public:
    void AddCommand(ICommand* command) override;

    void ExecuteCommands(IEntityManager* entityManager, ComponentManager* componentManager,
                         ComponentManager* renderingComponentManager);

private:
    std::vector<unique_ptr<ICommand>> m_commands;
};

} // namespace hatcher