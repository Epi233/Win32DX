#pragma once

#include "../DefinwWindows.h"
#include <queue>
#include <bitset>

class Keyboard
{
public:
    class Event
    {
    public:
        enum class Type { Invalid, Press, Release };

    private:
        Type _type;
        unsigned char _code;

    public:
        Event();
        Event(Type type, unsigned char code);
        bool IsPressed() const;
        bool IsReleased() const;
        bool IsValid() const;
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
    void clearKey();

    wchar_t readCharW();
    bool isCharEmpty() const;
    void clearChar();
    
    void clear();

    void enableAutoRepeat();
    void disableAutoRepeat();
    bool isAutoRepeatEnabled() const;

public:
    void onKeyPressed(unsigned char keycode);
    void onKeyReleased(unsigned char keycode);
    void onCharW(wchar_t c);
    void clearState();

private:
    static constexpr unsigned int NUM_OF_KEYS = 256;
    static constexpr unsigned int QUEUE_SIZE = 16;

private:
    bool _autoRepeat = false;
    std::bitset<NUM_OF_KEYS> _keyStateSet;
    std::queue<Event> _keyBuffer;
    std::queue<wchar_t> _charBuffer;
};
