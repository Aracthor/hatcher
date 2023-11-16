#include "DemoApplication.hpp"

#include <iostream>

#include "hatcher/Entity.hpp"
#include "hatcher/World.hpp"

using namespace hatcher;

DemoApplication::DemoApplication(const char* commandName)
    : GameApplication(commandName)
{
    CreateNewWorld();

    StartRendering("hatcher - demo", 800, 600);
}

DemoApplication::~DemoApplication() = default;
