#pragma once

#include "unique_ptr.hpp"

namespace hatcher
{
class Clock;

class UpdateTicker
{
public:
    UpdateTicker(float tickrate);
    ~UpdateTicker();

    int TickCount();

    void SetTickrate(float tickrate);
    float GetTickrate() const;

private:
    float m_tickTimeMs;
    float m_accumulatedTimeMs = 0.f;
    unique_ptr<Clock> m_clock;
};

} // namespace hatcher
