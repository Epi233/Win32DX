#pragma once

#include <queue>

class Mouse
{
public:
    class Event
    {
    public:
        enum class Type
        {
            Invalid,
            Move,
            LeftPress,
            LeftRelease,
            RightPress,
            RightRelease,
            WheelUp,
            WheelDown,
            MiddlePress,
            MiddleRelease,
            Enter,
            Leave
        };

    private:
        Type _type;
        int _x;
        int _y;

    public:
        Event();
        Event(Type t, int x, int y);

    public:
        bool isValid() const;
        Type getType() const;
        std::pair<int, int> getPosition() const;
        int getPositionX() const;
        int getPositionY() const;
    };

public:
    Mouse() = default;
    ~Mouse() = default;
    Mouse(const Mouse& mouse) = delete;
    Mouse& operator= (const Mouse& mouse) = delete;

public:
    std::pair<int, int> getPosition() const;
    int getPositionX() const;
    int getPositionY() const;
    bool isLeftPressed() const;
    bool isMiddlePressed() const;
    bool isRightPressed() const;
    bool isInWindow() const;
    Event raiseEvent();
    bool isEmpty() const;
    void clear();

public:
    void onMouseMove(int x, int y);
    
    void onLeftMousePressed(int x, int y);
    void onLeftMouseReleased(int x, int y);
    
    void onMiddleMousePressed(int x, int y);
    void onMiddleMouseReleased(int x, int y);
    
    void onRightMousePressed(int x, int y);
    void onRightMouseReleased(int x, int y);
    
    void onWheelUp(int x, int y);
    void onWheelDown(int x, int y);
    void OnWheelDelta(int x, int y, int wheelDelta);

    void onMouseEnter();
    void onMouseLeave();

private:
    static constexpr unsigned int QUEUE_SIZE = 16;

private:
    int _x;
    int _y;
    bool _leftPressed;
    bool _middlePressed;
    bool _rightPressed;
    int _wheelDelta;
    bool _isMouseInWindow;
    std::queue<Event> _buffer;
};
