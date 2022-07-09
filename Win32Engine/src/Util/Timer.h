#pragma once

#include <chrono>

class Timer
{
public:
    Timer();

public:
    void Tik();
    float Toc();
    float Mark();

private:
    std::chrono::steady_clock::time_point _timeStamp;
};
