#pragma once

#include <chrono>

namespace hatcher
{

class Clock
{
public:
    Clock();

    void Update();

    float GetElapsedTime() const { return m_elapsedTime; }

private:
    using ClockType = std::chrono::system_clock;

    std::chrono::time_point<ClockType> m_previousTime;
    std::chrono::time_point<ClockType> m_lastTime;
    float m_elapsedTime = 0.f;
};

} // namespace hatcher
