#include "Application.h"

Application::Application(int windowWidth, int windowHeight, const wchar_t* windowName)
    : _window(windowWidth, windowHeight, windowName)
{
}

int Application::begin()
{
    while (true)
    {
        if (const auto msgResult = Window::processMessage())
        {
            return *msgResult;
        }

        gameLoop();
    }
}

void Application::gameLoop()
{
    _window.graphicSwapChain();
}
