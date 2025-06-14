#include "UpdateTicker.hpp"

#include "Clock.hpp"

#include <cmath>

namespace hatcher
{

UpdateTicker::UpdateTicker(float tickrate)
    : m_tickTimeMs(1000.f / tickrate)
    , m_clock(new Clock())
{
}

int UpdateTicker::TickCount()
{
    m_accumulatedTimeMs += m_clock->GetElapsedTime();
    m_clock->Update();
    const int tickCount = static_cast<int>(m_accumulatedTimeMs / m_tickTimeMs);
    m_accumulatedTimeMs = std::fmod(m_accumulatedTimeMs, m_tickTimeMs);
    return tickCount;
}

} // namespace hatcher
