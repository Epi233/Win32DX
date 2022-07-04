#include "Keyboard.h"

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


