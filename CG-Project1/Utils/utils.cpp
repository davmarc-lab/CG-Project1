#include "utils.hpp"

bool Helper::checkInRoadBound(float y)
{
    return y < (float)this->wheight / 2 - 50 - 25 && y > 0 + 50 + 25;
}

void Helper::enemyMoveAction(ComplexShape2D* entity)
{
    float velocity = 0.1;
    int pixel = 1;
    vec3 pos = vec3(entity->getModelMatrix()[3]);

    if (!checkInRoadBound(pos.y))
        this->startDirection *= -1;
    entity->translateShape(vec3(0, startDirection * pixel * velocity, 0));
}
