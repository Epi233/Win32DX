#pragma once

template<typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T inst;
        return inst;
    }
    
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton&) = delete;
};
