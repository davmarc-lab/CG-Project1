#include "Game/Game.hpp"
#include "Lib.hpp"
#include "Window/Window.hpp"
#include "Game/Game.hpp"

#include "ImGui.hpp"
#include "include/imgui/imgui.h"

#define PI 3.14159265358979323846

const int WIDTH = 1600, HEIGHT = 900;

int main() {

    // Instance the game object
    Game game = Game(WIDTH, HEIGHT);
    // Instance the window
    Window window = Window(GAME_NAME, WIDTH, HEIGHT);

    // enable imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init();

    // time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Initialize menu screen
    game.initMenu();

    // glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Game main menu load
    while (!glfwWindowShouldClose(window.getWindow()) && game.getState() == GameState::GAME_MENU) {
        // manage the time between each frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process user input
        window.processCloseInput();
        game.processMouseInput(deltaTime, window);

        game.updateMenu(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render
        glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.renderMenu();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // render main menu
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }
    game.clearMenu();

    // exit button pressed
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
        
        // input
        game.processGameInput(deltaTime, window);

        // game not in PAUSE phase
        if (game.getState() != GameState::GAME_PAUSE)
        {
            // update game state
            game.updateGame(deltaTime);
        }

        glClearColor(0.78f, 0.96f, 0.94f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // renders all the game elements
        game.renderGame();

        // input for closing window
        window.processCloseInput();
        
        // swap buffers
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }
    // clear all game entities and exit
    game.clearGame();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.terminateWindow();
    return 0;
}
