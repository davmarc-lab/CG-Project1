#pragma once

#include "../Lib.hpp"
#include "../Window/Window.hpp"

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

        void init();
        
        void processInput(float deltaTime, Window window);
        
        void update(float deltaTime);
        
        void render();

        void clear();

        GameState getState() { return this->state; }
};

