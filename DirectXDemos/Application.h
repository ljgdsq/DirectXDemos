#pragma once

#include "Window.h"
#include "Timer.h"
class Application
{
public:
    Application() noexcept;
    int Run();
private:
    void DoFrame();
private:
    Window window;
    Timer timer;
};

