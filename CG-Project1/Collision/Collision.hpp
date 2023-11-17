#pragma once

#include "../Lib.hpp"

#include <vector>

class Collision
{
    private:
        bool collision; 

    public:
        Collision() { this->collision = false; }

        bool isColliding() { return this->collision; }
};
