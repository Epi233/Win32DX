#pragma once

#include "../DefinwWindows.h"
#include <queue>
#include <bitset>

class Keyboard
{
    friend class Windows;

public:
    class Event
    {
    public:
        enum class Type { Invalid, Press, Release };

    private:
        Type _type;
        unsigned char _code;

    public:
        
    };
    
public:
    Keyboard() = default;
    ~Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator= (const Keyboard&) = delete;

public:
    bool isKeyPressed(unsigned char keycode) const;
    Event readKey();
    bool isKeyEmpty() const;
    void clear();

    wchar_t readCharW();
    bool isCharEmpty() const;
    void flushChar();
    void flush();

    void enableAutoRepeat();
    void disableAutoRepeat();
    bool isAutoRepeatEnabled();

private:
    void onKeyPressed(unsigned char keycode);
    void onKeyReleased(unsigned char keycode);
    void onCharW(wchar_t c);
    void clearState();

private:
    template<typename T>
    static void TrimQueue(std::queue<T> q);

private:
    static constexpr unsigned int NUM_OF_KEYS = 256;
    static constexpr unsigned int QUEUE_SIZE = 16;

private:
    bool _autoRepeat = false;
    std::bitset<NUM_OF_KEYS> _keyStateSet;
    std::queue<Event> _keyBuffer;
    std::queue<wchar_t> _charBuffer;
};
