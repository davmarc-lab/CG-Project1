#pragma once

#include "../Shape/ComplexShape2D.hpp"

#define PI 3.14159265358979323846

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

        static void buildCircle(float cx, float cy, float raggiox, float raggioy, ComplexShape2D* fig);
};

