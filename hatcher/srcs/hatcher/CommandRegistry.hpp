#pragma once

#include <functional>

#include "basic_types.hpp"

namespace hatcher
{
class ICommand;

using CommandCreator = std::function<ICommand*()>;
void RegisterCommand(uint identifier, CommandCreator commandCreator);

} // namespace hatcher
