#pragma once

#include "../Lib.hpp"

class Collision
{
    private:
        bool collision; 

    public:
        Collision() { this->collision = false; }

        bool isColliding() { return this->collision; }
};
