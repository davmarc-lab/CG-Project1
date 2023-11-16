#pragma once

#include "../Shape/ComplexShape2D.hpp"

class Helper
{
    int wheight;
    int wwidth;
    int startDirection;
    float yvelocity;
    public:
        Helper(vec2 res) {
            this->wwidth = res.x;
            this->wheight = res.y;
            this->startDirection = 1;
            this->yvelocity = 0.1f;
        }

        void setYVelocity(float val) { this->yvelocity = val; }
        
        bool checkInRoadBound(float y);

        void enemyMoveAction(ComplexShape2D* entity);
};

