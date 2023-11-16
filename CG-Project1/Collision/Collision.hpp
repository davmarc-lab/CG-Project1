#pragma once

#include "../Lib.hpp"

#include <glm/fwd.hpp>
#include <type_traits>
#include <vector>

class Collision
{
    private:
        vector<vec3> points;

    public:
        Collision();

        bool checkCollision2D(vec2 point);

        bool checkCollision3D(vec3 point);
};
