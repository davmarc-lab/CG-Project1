#include "Game/Game.hpp"
#include "Lib.hpp"
#include "Shape/ComplexShape2D.hpp"
#include "Window/Window.hpp"
#include "Game/Game.hpp"

#include <iostream>

#define PI 3.14159265358979323846

const int WIDTH = 1600, HEIGHT = 900;

int main()
{

    Game game = Game(WIDTH, HEIGHT);
    Window window = Window(GAME_NAME, WIDTH, HEIGHT);

    // time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    /* // Initialize menu screen */
    game.initMenu();

    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Game main menu load
    while (!glfwWindowShouldClose(window.getWindow()) && game.getState() == GameState::GAME_MENU)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // process user input
        window.processCloseInput();
        game.processMouseInput(deltaTime, window);

        // render
        glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.renderMenu();
        game.updateMenu(deltaTime);

        // render main menu
        glfwSwapBuffers(window.getWindow());
    }
    game.clearMenu();

    if (game.getState() == GameState::GAME_END)
    {
        return 0;
    }

    // Initialize all game object and window
    game.initGame();

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
        game.processGameInput(deltaTime, window);

        // update game state
        game.updateGame(deltaTime);

        // render
        glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.renderGame();

        // swap buffers
        glfwSwapBuffers(window.getWindow());
    }
    game.clearGame();
    window.terminateWindow();
    return 0;
}
