#pragma once

#include "ClassKey.hpp"
#include "CommandRegistry.hpp"
#include "ICommand.hpp"

#define COMMAND_HEADER(COMMAND_NAME)                                                                                   \
                                                                                                                       \
public:                                                                                                                \
    COMMAND_NAME() = default;                                                                                          \
                                                                                                                       \
    uint Identifier() const override                                                                                   \
    {                                                                                                                  \
        return ClassKey<COMMAND_NAME>();                                                                               \
    }

namespace hatcher
{

template <class CommandType>
class CommandRegisterer
{
public:
    CommandRegisterer()
    {
        RegisterCommand(ClassKey<CommandType>(), []() { return new CommandType(); });
    }
};

} // namespace hatcher

#define REGISTER_COMMAND(COMMAND_NAME) CommandRegisterer<COMMAND_NAME> registerer_##COMMAND_NAME;