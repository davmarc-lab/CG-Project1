#pragma once

#include <string.h>
#include "../Lib.hpp"

/*
 * This class instance a GLFWwindow with a default close option.
 */
class Window
{
    private:
        GLFWwindow *window;
        int width;
        int height;
        const char *windowName;
        vec2 resolution;

    public:
        // Constructs a window with window name, width and height given.
        Window(const char *windowName, int w, int h);

        // Initialize the window.
        int initializeWindow();

        // Closes the window.
        void closeWindow();

        // Terminates the window process and clears automatically all buffers used.
        void terminateWindow();

        // Callback function for close window input.
        void processCloseInput();

        // Retrieves the window name.
        char *getWindowName() { return (char *)this->windowName; };

        // Retrieves the GLFWwindow window.
        GLFWwindow *getWindow() { return this->window; };

        // Retrieves the resolution of the window.
        vec2 getResolution() { return this->resolution; }

};
