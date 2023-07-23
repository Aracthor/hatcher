#pragma once

namespace hatcher
{
class ICommand;

class ICommandManager
{
public:
    virtual void AddCommand(ICommand* command) = 0;
};

} // namespace hatcher