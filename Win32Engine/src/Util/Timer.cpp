#include "Timer.h"

Timer::Timer()
{
    Tik();
}

void Timer::Tik()
{
    _timeStamp = std::chrono::steady_clock::now();
}

float Timer::Toc()
{
    auto lastTime = _timeStamp;
    _timeStamp = std::chrono::steady_clock::now();
    const std::chrono::duration<float> deltaTime = _timeStamp - lastTime;
    return deltaTime.count();
}

float Timer::Mark()
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - _timeStamp).count();
}



