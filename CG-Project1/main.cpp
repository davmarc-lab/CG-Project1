#include "Game/Game.hpp"
#include "Lib.hpp"
#include "Shape/ComplexShape2D.hpp"
#include "Window/Window.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#define PI 3.14159265358979323846

const int WIDTH = 1600, HEIGHT = 900;

int main()
{

    Game game = Game(WIDTH, HEIGHT);
    Window window = Window("Mario Kart", WIDTH, HEIGHT);

    // Initialize all game object and window
    game.init();

    // time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Start of window loop
    while (!glfwWindowShouldClose(window.getWindow()) && game.getState() != GameState::GAME_END)
    {
        // manage frames time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // input
        window.processCloseInput();
        game.processInput(deltaTime, window);

        // update game state
        game.update(deltaTime);

        // render
        glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.render();

        // swap buffers and poll IO events
        glfwSwapBuffers(window.getWindow());
    }
    game.clear();
    window.terminateWindow();
    return 0;
}
