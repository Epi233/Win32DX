#include "Mouse.h"

#include "../Console/DebugConsole.h"
#include "../Util/Util.h"

Mouse::Event::Event()
    : _type(Type::Invalid)
    , _x(0)
    , _y(0)
{
}

Mouse::Event::Event(Type t, int x, int y)
    : _type(t)
    , _x(x)
    , _y(y)
{
}

bool Mouse::Event::isValid() const
{
    return _type != Type::Invalid;
}

Mouse::Event::Type Mouse::Event::getType() const
{
    return _type;
}

std::pair<int, int> Mouse::Event::getPosition() const
{
    return {_x, _y};
}

int Mouse::Event::getPositionX() const
{
    return _x;
}

int Mouse::Event::getPositionY() const
{
    return _y;
}

std::pair<int, int> Mouse::getPosition() const
{
    return {_x ,_y};
}

int Mouse::getPositionX() const
{
    return _x;
}

int Mouse::getPositionY() const
{
    return _y;
}

bool Mouse::isLeftPressed() const
{
    return _leftPressed;
}

bool Mouse::isMiddlePressed() const
{
    return _middlePressed;
}

bool Mouse::isRightPressed() const
{
    return _rightPressed;
}

bool Mouse::isInWindow() const
{
    return _isMouseInWindow;
}

Mouse::Event Mouse::raiseEvent()
{
    if (!_buffer.empty())
    {
        auto evt = _buffer.front();
        _buffer.pop();
        return evt;
    }
    else
    {
        return Event();
    }
}

bool Mouse::isEmpty() const
{
    return _buffer.empty();
}

void Mouse::clear()
{
    while (!_buffer.empty())
        _buffer.pop();
}

void Mouse::onMouseMove(int x, int y)
{
    _x = x;
    _y = y;

    _buffer.push(Event(Event::Type::Move, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onLeftMousePressed(int x, int y)
{
    _x = x;
    _y = y;
    _leftPressed = true;

    _buffer.push(Event(Event::Type::LeftPress, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onLeftMouseReleased(int x, int y)
{
    _x = x;
    _y = y;
    _leftPressed = false;

    _buffer.push(Event(Event::Type::LeftRelease, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onMiddleMousePressed(int x, int y)
{
    _x = x;
    _y = y;
    _middlePressed = true;

    _buffer.push(Event(Event::Type::MiddlePress, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onMiddleMouseReleased(int x, int y)
{
    _x = x;
    _y = y;
    _middlePressed = false;

    _buffer.push(Event(Event::Type::MiddleRelease, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onRightMousePressed(int x, int y)
{
    _x = x;
    _y = y;
    _rightPressed = true;

    _buffer.push(Event(Event::Type::RightPress, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onRightMouseReleased(int x, int y)
{
    _x = x;
    _y = y;
    _rightPressed = true;

    _buffer.push(Event(Event::Type::RightRelease, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onWheelUp(int x, int y)
{
    _x = x;
    _y = y;

    _buffer.push(Event(Event::Type::WheelUp, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::onWheelDown(int x, int y)
{
    _x = x;
    _y = y;

    _buffer.push(Event(Event::Type::WheelDown, x, y));
    Util::TrimQueue(_buffer, QUEUE_SIZE);
}

void Mouse::OnWheelDelta(int x, int y, int wheelDelta)
{
    _wheelDelta += wheelDelta;

    // 根据文档，应该每120产生一个事件
    while (_wheelDelta >= WHEEL_DELTA)
    {
        _wheelDelta -= WHEEL_DELTA;
        onWheelUp(x, y);
    }
    while (_wheelDelta <= -WHEEL_DELTA)
    {
        _wheelDelta += WHEEL_DELTA;
        onWheelDown(x, y);
    }
}

void Mouse::onMouseEnter()
{
    _isMouseInWindow = true;
}

void Mouse::onMouseLeave()
{
    _isMouseInWindow = false;
}


