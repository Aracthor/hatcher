#include "Clock.hpp"

namespace hatcher
{

Clock::Clock()
    : m_previousTime(ClockType::now())
    , m_lastTime(ClockType::now())
{
}

void Clock::Update()
{
    m_previousTime = m_lastTime;
    m_lastTime = ClockType::now();
    std::chrono::microseconds microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(m_lastTime - m_previousTime);
    m_elapsedTime = static_cast<float>(microseconds.count()) / 1000.f;
}

} // namespace hatcher
