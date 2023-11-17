#include "utils.hpp"

bool Helper::checkInRoadBound(float y)
{
    return y < (float)this->wheight / 2 - 50 - 25 && y > 0 + 50 + 25;
}

void Helper::enemyMoveAction(ComplexShape2D* entity)
{
    int pixel = 1;
    vec3 pos = vec3(entity->getModelMatrix()[3]);

    if (!checkInRoadBound(pos.y))
        this->startDirection *= -1;
    entity->translateShape(vec3(0, startDirection * pixel * this->yvelocity, 0));
}

void Helper::buildCircle(float cx, float cy, float raggiox, float raggioy, ComplexShape2D* fig)
{
    float stepA = (2 * PI) / fig->getTriangleNum();
    float t, xx, yy;

    fig->addElementVertex(vec3(cx, cy, 0.0));

    fig->addElementColors(fig->getMidColor().getColorVector());

    for (int i = 0; i <= fig->getTriangleNum(); i++)
    {
        t = (float)i * stepA;
        xx = cx + raggiox * cos(t);
        yy = cy + raggioy * sin(t);

        fig->addElementVertex(vec3(xx, yy, 0.0));
        fig->addElementColors(fig->getColor().getColorVector());
    }
    fig->setVertexNum(fig->getTriangleNum());
}
