#pragma once

#include <memory>

namespace hatcher
{

class IEventListener;

class IEventUpdater
{
public:
    virtual void RegisterListener(const std::shared_ptr<IEventListener>& eventListener) = 0;
};

} // namespace hatcher
