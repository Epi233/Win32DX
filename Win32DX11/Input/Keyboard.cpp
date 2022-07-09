#include "Keyboard.h"
#include "../Util/Util.h"

Keyboard::Event::Event()
    : _type(Type::Invalid)
    , _code(0)
{
}

Keyboard::Event::Event(Type type, unsigned char code)
    : _type(type)
    , _code(code)
{
}

bool Keyboard::Event::IsPressed() const
{
    return _type == Type::Press;
}

bool Keyboard::Event::IsReleased() const
{
    return _type == Type::Release;
}

bool Keyboard::Event::IsValid() const
{
    return _type != Type::Invalid;
}

Keyboard::Event Keyboard::readKey()
{
    if (!_keyBuffer.empty())
    {
        auto keyboardEvent = _keyBuffer.front();
        _keyBuffer.pop();
        return keyboardEvent;
    }
    else
    {
        return Keyboard::Event();
    }
}

bool Keyboard::isKeyPressed(unsigned char keycode) const
{
    return _keyStateSet[keycode];
}

bool Keyboard::isKeyEmpty() const
{
    return _keyBuffer.empty();
}

void Keyboard::clearKey()
{
    while (!_keyBuffer.empty())
    {
        _keyBuffer.pop();
    }
}

wchar_t Keyboard::readCharW()
{
    if (!_charBuffer.empty())
    {
        wchar_t top = _charBuffer.front();
        _charBuffer.pop();
        return top;
    }

    return 0;
}

bool Keyboard::isCharEmpty() const
{
    return _charBuffer.empty();
}

void Keyboard::clearChar()
{
    while (!_charBuffer.empty())
    {
        _charBuffer.pop();
    }
}

void Keyboard::clear()
{
    clearKey();
    clearChar();
}

void Keyboard::enableAutoRepeat()
{
    _autoRepeat = true;
}

void Keyboard::disableAutoRepeat()
{
    _autoRepeat = false;
}

bool Keyboard::isAutoRepeatEnabled() const
{
    return _autoRepeat;
}

void Keyboard::onKeyPressed(unsigned char keycode)
{
    _keyStateSet[keycode] = true;
    _keyBuffer.push(Event(Event::Type::Press, keycode));
    Util::TrimQueue(_keyBuffer, QUEUE_SIZE);
}

void Keyboard::onKeyReleased(unsigned char keycode)
{
    _keyStateSet[keycode] = false;
    _keyBuffer.push(Event(Event::Type::Release, keycode));
    Util::TrimQueue(_keyBuffer, QUEUE_SIZE);
}

void Keyboard::onCharW(wchar_t c)
{
    _charBuffer.push(c);
    Util::TrimQueue(_charBuffer, QUEUE_SIZE);
}

void Keyboard::clearState()
{
    _keyStateSet.reset();
}




