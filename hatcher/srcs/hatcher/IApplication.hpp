#pragma once

namespace hatcher
{

class IApplication
{
public:
    virtual ~IApplication() = default;
    virtual void Stop() = 0;
    virtual void SetUpdateTickrate(float tickrate) = 0;
};

} // namespace hatcher