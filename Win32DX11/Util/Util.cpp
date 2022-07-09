#include "Util.h"

template <typename T>
void Util::TrimQueue(std::queue<T> q, int size)
{
    while (q.size() > size)
        q.pop();
}
