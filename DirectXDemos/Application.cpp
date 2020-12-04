#include "Application.h"

Application::Application() noexcept :window(300, 200, "dx")
{
}

int Application::Run()
{
    while (true)
    {
        if (const auto rt = Window::ProcessMessage())
        {
            return *rt;
        }
        //timer.Mark();
        DoFrame();
    }

    return -1;
}



void Application::DoFrame()
{
    const float r = (float)(sin(timer.Peek()) / 2.0f) + 0.5f;
    window.Gfx().ClearBuffer(r, 0.5f, 1 - r);
    window.Gfx().DrawTestShape(timer.Peek());
    window.Gfx().EndFrame();
  
 
}
