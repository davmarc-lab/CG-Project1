#include "utils.hpp"
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void Helper::enemyMoveAction(vec3 direction, ComplexShape2D* entity)
{
    vec3 pos = vec3(entity->getModelMatrix()[3]);

    direction = direction * this->velocity;
    entity->setModelMatrix(translate(entity->getModelMatrix(), vec3(pos.x + direction.x, pos.y + direction.y, 0)));
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

void Helper::buildTriangle(ComplexShape2D* fig)
{
    fig->addElementVertex(vec3(-1, -1, 0));
    fig->addElementColors(fig->getColor().getColorVector());
    fig->addElementVertex(vec3(1, -1, 0));
    fig->addElementColors(fig->getColor().getColorVector());
    fig->addElementVertex(vec3(0, 1, 0));
    fig->addElementColors(fig->getColor().getColorVector());

    fig->setVertexNum(3);
    fig->setTriangleNum(1);
}

vec2 Helper::getRandomPosition2D(pair<int, int> xrange, pair<int, int> yrange)
{
    int xval = xrange.first - xrange.second == 0 ? xrange.first : rand() % (abs(xrange.first - xrange.second) + 1 + std::min(xrange.first, xrange.second));
    int yval = yrange.first - yrange.second == 0 ? yrange.first : (rand() % (abs(yrange.first - yrange.second)) + 1 + std::min(yrange.first, yrange.second)); 
    return vec2(xval, yval);
}
