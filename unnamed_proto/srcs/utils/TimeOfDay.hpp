#pragma once

constexpr float tickPerMinute = 60.f;
constexpr float minutePerHour = 60.f;
constexpr float hourPerDay = 24.f;

struct TimeOfDay
{
    int day;
    int hour;
    int minute;
};

constexpr int MinutesToTicks(float minutes)
{
    return static_cast<int>(minutes * tickPerMinute);
}

constexpr int HoursToTicks(float hours)
{
    return MinutesToTicks(hours * minutePerHour);
}

constexpr int DaysToTicks(float days)
{
    return HoursToTicks(days * hourPerDay);
}

constexpr int TicksToMinutes(int ticks)
{
    return ticks / tickPerMinute;
}

constexpr int TicksToHours(int ticks)
{
    return TicksToMinutes(ticks) / minutePerHour;
}

constexpr int TicksToDays(int ticks)
{
    return TicksToHours(ticks) / hourPerDay;
}

constexpr TimeOfDay GetTimeOfDay(int ticks)
{
    return {
        .day = TicksToDays(ticks) + 1,
        .hour = TicksToHours(ticks % DaysToTicks(1)),
        .minute = TicksToMinutes(ticks % HoursToTicks(1)),
    };
}