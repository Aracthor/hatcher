#pragma once

#include "hatcher/GameApplication.hpp"

using namespace hatcher;

class DemoApplication final : public GameApplication
{
public:
    DemoApplication(const char* commandName);
    ~DemoApplication();
};
