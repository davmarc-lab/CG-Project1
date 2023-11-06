#pragma once

#include <string.h>
#include "../Lib.hpp"

class Window
{
private:
    GLFWwindow *window;
    int width;
    int height;
    const char *windowName;

public:
    Window(const char *windowName, int w, int h);

    int initializeWindow();

    void closeWindow();

    void terminateWindow();

    void processCloseInput();

    char *getWindowName() { return (char *)this->windowName; };

    GLFWwindow *getWindow() { return this->window; };

};