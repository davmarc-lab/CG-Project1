#pragma once

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
    public:
        GameState state;
        unsigned int width, height;
        Game(unsigned int width, unsigned int height);

        void init();
        void processInput(float deltaTime);
        void update(float deltaTime);
        void render();
};

