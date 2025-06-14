#pragma once

#include "unique_ptr.hpp"

namespace hatcher
{
class Clock;

class UpdateTicker
{
public:
    UpdateTicker(float tickrate);

    int TickCount();

private:
    const float m_tickTimeMs;
    float m_accumulatedTimeMs = 0.f;
    unique_ptr<Clock> m_clock;
};

} // namespace hatcher
