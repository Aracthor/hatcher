#include "UpdateTicker.hpp"

#include "Clock.hpp"

#include <cmath>
#include <cstdio>

namespace hatcher
{

UpdateTicker::UpdateTicker(float tickrate)
    : m_clock(new Clock())
{
    SetTickrate(tickrate);
}

UpdateTicker::~UpdateTicker() = default;

int UpdateTicker::TickCount()
{
    if (m_tickTimeMs == 0.f)
    {
        m_clock->Update();
        return 0;
    }

    m_accumulatedTimeMs += m_clock->GetElapsedTime();
    m_clock->Update();
    const int tickCount = static_cast<int>(m_accumulatedTimeMs / m_tickTimeMs);
    m_accumulatedTimeMs = std::fmod(m_accumulatedTimeMs, m_tickTimeMs);
    return tickCount;
}

void UpdateTicker::SetTickrate(float tickrate)
{
    m_tickTimeMs = tickrate > 0.f ? 1000.f / tickrate : 0.f;
}

float UpdateTicker::GetTickrate() const
{
    return m_tickTimeMs > 0.f ? 1000.f / m_tickTimeMs : 0.f;
}

} // namespace hatcher
