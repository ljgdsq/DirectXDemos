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


#include <sstream>
void Application::DoFrame()
{
    //std::stringstream ss;
    //ss << timer.Peek();
    //window.SetTitle(ss.str().c_str());
}
