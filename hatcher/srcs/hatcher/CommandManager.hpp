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

    span<const unique_ptr<ICommand>> CurrentCommands() const { return span<const unique_ptr<ICommand>>(m_commands); }

    void ExecuteCommands(IEntityManager* entityManager, ComponentManager* componentManager);

private:
    std::vector<unique_ptr<ICommand>> m_commands;
};

} // namespace hatcher