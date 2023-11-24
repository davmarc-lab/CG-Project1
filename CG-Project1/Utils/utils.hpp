#pragma once

#include "../Shape/ComplexShape2D.hpp"

#include <cstdlib>

#define PI 3.14159265358979323846

class Helper
{
    int wheight;
    int wwidth;
    int startDirection;
    float velocity;
    public:
        Helper(vec2 res) {
            this->wwidth = res.x;
            this->wheight = res.y;
            this->startDirection = 1;
            this->velocity = 0.1f;
        }

        float getVelocity() { return this->velocity; }

        void setVelocity(float val) { this->velocity = val; }
        
        void enemyMoveAction(vec3 direction, ComplexShape2D* entity);

        static void buildCircle(float cx, float cy, float raggiox, float raggioy, ComplexShape2D* fig);

        static void buildTriangle(ComplexShape2D* fig);

        // Generates random 2D coordinates by given ranges. If a range is (n, n), it retrieves
        // only the n number without doing random generation.
        vec2 static getRandomPosition2D(pair<int, int> xrange, pair<int, int> yrange);

};

