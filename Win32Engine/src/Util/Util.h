#pragma once

#include <queue>

class Util
{
public:
    template<typename T>
    static void TrimQueue(std::queue<T> q, int size)
    {
        while (q.size() > size)
            q.pop();
    }
};

