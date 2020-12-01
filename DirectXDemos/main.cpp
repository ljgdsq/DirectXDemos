#include "ChiliWin.h"
#include <string>
#include <sstream>

#include "Window.h"
#include "Utils/WindowsMessageMap.h"


int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPreInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    Window wind(300,200,"dx");
   

    MSG msg;

    BOOL gResult;

    while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (gResult != -1)
    {
        return msg.wParam;
    }



    return -1;
}