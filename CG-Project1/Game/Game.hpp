#pragma once

#include "../Lib.hpp"
#include "../Window/Window.hpp"

// Window name
#define GAME_NAME "Kart"

/*
 * Enum to manage game phases.
 */
enum GameState
{
    GAME_NONE,
    GAME_MENU,
    GAME_ACTIVE,
    GAME_PAUSE,
    GAME_END
};

/*
 * This class create a game object.
 */
class Game
{
    private:
        bool isOutOfBounds(vec2 pos);
    public:
        GameState state;
        unsigned int width, height;

        Game(unsigned int width, unsigned int height);

        /*
         * Initialize all game entities.
         */
        void initGame();

        /*
         * Initialize the menu entities.
         */
        void initMenu();

        /*
         * Process all user inputs during game phase.
         */
        void processGameInput(float deltaTime, Window window);

        // Process mouse input.
        void processMouseInput(float deltaTime, Window window);

        // Updates all entities elements/states during the game.
        void updateGame(float deltaTime);

        // Updates all entities states during menu phase.
        void updateMenu(float deltaTime);

        // Render all game entities.
        void renderGame();

        // Render all menu entities.
        void renderMenu();

        // Clears game entities.
        void clearGame();

        // Clears menu entities.
        void clearMenu();

        // Retrieves game current state.
        GameState getState() { return this->state; }
};

