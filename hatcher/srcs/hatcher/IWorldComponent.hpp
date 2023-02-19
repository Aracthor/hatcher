#pragma once

namespace hatcher
{

class ISaveLoader;

class IWorldComponent
{
public:
    virtual ~IWorldComponent() = default;

    virtual void SaveLoad(ISaveLoader& saveLoader) = 0;
    virtual void PostLoad(){};
};

} // namespace hatcher
