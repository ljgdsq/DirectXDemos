#include "Mouse.h"
#include "Window.h"

#include <iostream>
#include <sstream>

#if _DEBUG
#define LOG_MOUSE(funcName,x,y) {\
    std::stringstream ss; \
    ss << #funcName   << " x:"<<x<<" y:"<<y <<std::endl;\
    OutputDebugString(ss.str().c_str()); \
    };

#define LOG_MOUSE_ACTION(funcName) {\
    OutputDebugString(#funcName); \
    };
#else
#define LOG_MOUSE(funcName,x,y) {};

#define LOG_MOUSE_ACTION(funcName) {};
#endif




std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x,y };
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::IsInWindow() const noexcept
{
    return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}


Mouse::Event Mouse::Read() noexcept
{
    if (buffer.size() > 0u)
    {
        Mouse::Event e = buffer.front();
        buffer.pop();
        return e;
    }
    else
    {
        return Mouse::Event();
    }
}

void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
    LOG_MOUSE(OnMouseMove, x, y);
    this->x = x;
    this->y = y;
    buffer.push(Mouse::Event(Mouse::Event::Type::Move,*this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
    LOG_MOUSE_ACTION(OnMouseLeave);
    isInWindow = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    LOG_MOUSE_ACTION(OnMouseEnter);

    isInWindow = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
    LOG_MOUSE_ACTION(OnLeftPressed);

    leftIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
    LOG_MOUSE_ACTION(OnLeftReleased);

    leftIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}


void Mouse::OnRightPressed(int x, int y) noexcept
{
    LOG_MOUSE_ACTION(OnRightPressed);

    rightIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
    LOG_MOUSE_ACTION(OnRightReleased);
    rightIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
    LOG_MOUSE_ACTION(OnWheelUp);

    buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
    LOG_MOUSE_ACTION(OnWheelDown);
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
    wheelDeltaCarry += delta;
    // generate events for every 120 
    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (wheelDeltaCarry <= -WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}