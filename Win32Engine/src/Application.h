#pragma once

#include "Win32UI/Window.h"

class Application
{
public:
    Application(int windowWidth, int windowHeight, const wchar_t* windowName);

public:
    int begin();

private:
    Window _window;

private:
    void gameLoop();
};
