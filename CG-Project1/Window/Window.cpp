#include <iostream>
#include <string.h>
#include "Window.hpp"

using namespace std;

/*
    This constructor creates a window with width w and height h
*/
Window::Window(const char *name, int w, int h)
{
    this->width = w;
    this->height = h;
    this->windowName = name;
    this->resolution = vec2(this->width, this->height);
}

/*
    Update vieport dimension, usually used after resize window event
*/
void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
}

int Window::initializeWindow()
{
    // initialize GLFW contents
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    // creates a GLFW window
    this->window = glfwCreateWindow(this->width, this->height, this->windowName, NULL, NULL);
    if (this->window == NULL)
    {
        cout << "Failed to create GLFW window." << endl;
        glfwTerminate();
        return -1;
    }

    // focusing the created window and set a callback funciton for updating render when resizing window
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

    // checks if glad library has been loaded correctly
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        glfwTerminate();
        return -2;
    }
    return 0;
}

void Window::closeWindow()
{
    // delete all vao, vbo, ebo arrays
    glfwSetWindowShouldClose(this->window, true);
}

void Window::terminateWindow()
{
    // terminate GLFW window
    glfwTerminate();
}

void Window::processCloseInput()
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        this->closeWindow();
    }
}
