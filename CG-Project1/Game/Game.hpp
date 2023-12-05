#pragma once

#include "../Lib.hpp"
#include "../Window/Window.hpp"

#define GAME_NAME "Kart"

enum GameState
{
    GAME_NONE,
    GAME_MENU,
    GAME_ACTIVE,
    GAME_END
};

class Game
{
    public:
        GameState state;
        unsigned int width, height;
        
        Game(unsigned int width, unsigned int height);

        void initGame();

        void initMenu();
        
        void processGameInput(float deltaTime, Window window);

        void processMouseInput(float deltaTime, Window window);
        
        void updateGame(float deltaTime);

        void updateMenu(float deltaTime);
        
        void renderGame();

        void renderMenu();

        void clearGame();

        bool isOutOfBounds(vec2 pos);

        GameState getState() { return this->state; }
};

